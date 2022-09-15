#include "../gxco/gxco.h"
#include "igemm_gtc_base.h"
#include "operations/global_memory.h"
#include "operations/dotx_mapping.h"

std::vector<int> _find_non_1_index_in_list(const std::vector<int>& src){
    std::vector<int> result;
    for(auto& element : src){
        if(element != 1) result.push_back(element);
    }

    return result;
}

class igemm_fwd_gtc_nchwc_t : gxco::generator_t{
    /* @details 
     *                    tensor a (wei)               tensor b (inp)
     * thread_lengths  :  1, 1,      1, ta_k-vec-c     1, 1,     tb_nb0, tb_vec-c,
     * cluster_lengths :  1, ca_ce,  1, ca_k1          1, cb_ce, 1,      cb_nb1,    
     * for a/b tensor, always load vec-c dimension first.
     */
public:
    // Prepare control blocks
    igemm_fwd_gtc_nchwc_t(const gxco::target_info_t& ti_, 
                          gxco::emitter_t& emitter_, 
                          const igemm_gtc_tunable_t& tunable_)
    : gxco::generator_t(ti_, emitter_), tunable(tunable_){
        // Control Blocks based on tunables
        assert(IGEMM_GTC_FEAT_MAGIC_DIVISION == true);
        global_load_in_t global_load_in(this);
        global_load_wei_t global_load_wei(this);

        thread_copy_index tc_index = get_thread_copy_index();
        in_thread_copy_ndim = tc_index.in_thread_copy_index.size();
        wei_thread_copy_ndim = tc_index.wei_thread_copy_index.size();
        assert(in_thread_copy_ndim < 3 && wei_thread_copy_ndim <3);

        /* TODO macro-like functors */

        coalescing_store_groups = tunable.coalescing_store_groups;
        if(tunable.fma_type == IGEMM_GTC_TUNABLE_FMA_TYPE_DLOPS){
            assert( (tunable.lanegroup_tile_m * tunable.lanegroup_repeat_m) % coalescing_store_groups == 0 );
            ctrl_dotx_mapping_t ctrl_dotx_mapping = get_ctrl_dotx_mapping_from_lanegroup_tile(
                                                    tunable.gemm_m_per_block, tunable.gemm_n_per_block,
                                                    tunable.lanegroup_tile_m, tunable.lanegroup_tile_n,
                                                    tunable.lanegroup_wave_m, tunable.lanegroup_wave_n,
                                                    tunable.block_size / (tunable.lanegroup_wave_m * tunable.lanegroup_wave_n * LANEGROUP_SIZE),
                                                    tunable.lanegroup_repeat_m, tunable.lanegroup_repeat_n,
                                                    tunable.precision, get_dotx_fma_instruction(ti.arch, tunable.precision));
            
            dotx_mapping = igemm_dotx_mapping_t{ctrl_dotx_mapping};
            
            ctrl_coalescing_store_dotx_t ctrl_coalescing_store;
            ctrl_coalescing_store.cdm = ctrl_dotx_mapping;
            ctrl_coalescing_store.coalescing_groups = coalescing_store_groups;
            ctrl_coalescing_store.precision = tunable.precision;

            ctrl_coalescing_store.vector_store_m = coalescing_store_dotx_get_optimal_vector_m(ctrl_dotx_mapping, tunable.vector_c, coalescing_store_groups);
            ctrl_coalescing_store.vector_fold_m = tunable.vector_c;
            ctrl_coalescing_store.block_size = tunable.block_size;
            
            ctrl_coalescing_store.div_v_const_func = div_v_const_func;
            ctrl_coalescing_store.div_rem_v_const_func = div_rem_v_const_func;
            ctrl_coalescing_store.mul_vi_func = mul_vi_func;
            ctrl_coalescing_store.mul_si_func = mul_si_func;

            coalescing_store = igemm_coalescing_store_dotx_t(ctrl_coalescing_store);
        }
        else if(tunable.fma_type == IGEMM_GTC_TUNABLE_FMA_TYPE_WMMA){
            // TODO WMMA algorithm implementation.
        }
        else{
            printf("igemm_fwd_gtc_nchwc supported FMA type: \n 1. dlops (navi21)\n 2. wmma  (navi31)\n");
            assert(0);
        }


    }

private:
    struct thread_lengths
    {
        int ta_k_vec_c;
        int tb_nb0;
        int tb_nb_vec_c;
    };

    struct cluster_lengths
    {
        int ca_ce;
        int ca_k;
        int cb_ce;
        int cb_nb1;
    };

    struct dims_lengths
    {
        int na_k_vec_c;
        int na_ce;
        int nb_ce;
        int nb_nb0;
        int nb_nb1_vec_c;
    };

    struct thread_copy_dims{
        std::vector<int> in_thread_copy_dims;
        std::vector<int> wei_thread_copy_dims;
    };

    struct thread_copy_index{
        std::vector<int> in_thread_copy_index;
        std::vector<int> wei_thread_copy_index;
    };

    thread_lengths get_thread_lengths(){
        std::vector<int> t_ta = tunable.tensor_a_thread_lengths;
        std::vector<int> t_tb = tunable.tensor_b_thread_lengths;
        assert(t_ta.size()==4 && t_tb.size()==4);
        assert(t_ta[0] == 1 && t_ta[1] == 1);

        int ta_k_vec_c = t_ta[3];
        int tb_nb0 = t_tb[2];
        int tb_nb_vec_c = t_tb[3];

        return thread_lengths(ta_k_vec_c, tb_nb0, tb_nb_vec_c);
    }

    cluster_lengths get_cluster_lengths(){
        std::vector<int> c_ta = tunable.tensor_a_cluster_lengths;
        std::vector<int> c_tb = tunable.tensor_b_cluster_lengths;
        assert(c_ta.size()==4 && c_tb.size()==4);
        assert(c_ta[0] == 1 && c_ta[2] == 1 && c_tb[0] == 1 && c_tb[2] == 1);

        int ca_ce  = c_ta[1];
        int ca_k   = c_ta[3];
        int cb_ce  = c_tb[1];
        int cb_nb1 = c_tb[3];

        return cluster_lengths{ca_ce, ca_k, cb_ce, cb_nb1};
    }

    dims_lengths get_dims_lengths(){
        thread_lengths tl = get_thread_lengths();
        cluster_lengths cl = get_cluster_lengths();

        int na_ce = cl.ca_ce;
        int na_k_vec_c = tl.ta_k_vec_c * cl.ca_k;
        int nb_ce = cl.cb_ce;
        int nb_nb0 = tl.tb_nb0;
        int nb_nb1_vec_c = tl.tb_nb_vec_c * cl.cb_nb1;

        return dims_lengths{na_ce, na_k_vec_c, nb_ce, nb_nb0, nb_nb1_vec_c};
    }

    thread_copy_dims get_thread_copy_dims(){
        thread_lengths tl = get_thread_lengths();
        std::vector<int> in_thread_copy_dims{tl.tb_nb0, tl.tb_nb_vec_c};
        std::vector<int> wei_thread_copy_dims{tl.ta_k_vec_c};
        
        return thread_copy_dims{in_thread_copy_dims, wei_thread_copy_dims};
    }

    thread_copy_index get_thread_copy_index(){
        thread_copy_dims tc_dims = get_thread_copy_dims();
        std::vector<int> in_thread_copy_index = _find_non_1_index_in_list(tc_dims.in_thread_copy_dims);
        std::vector<int> wei_thread_copy_index = _find_non_1_index_in_list(tc_dims.wei_thread_copy_dims);

        return thread_copy_index{in_thread_copy_index, wei_thread_copy_index};
    }


    macro_base_t get_macro_global_load(){
        bool _inline = tunable.fma_interleave ? true : false;
        thread_lengths tl = get_thread_lengths();
        cluster_lengths cl = get_cluster_lengths();
        dims_lengths dl = get_dims_lengths();

        thread_copy_dims tc_dims = get_thread_copy_dims();
        thread_copy_index tc_index = get_thread_copy_index();
        ctrl_2d_global_load_t ctrl_wei_gld;
        ctrl_2d_global_load_t ctrl_in_gld;

        float data_byte = amdgpu_precision_data_byte(tunable.precision);

        ctrl_wei_gld.precision = ctrl_in_gld.precision = tunable.precision;
        ctrl_in_gld.arch_name = ctrl_wei_gld.arch_name = amdgpu_string_to_arch(ti.arch);
        ctrl_wei_gld.vector_d1 = ctrl_in_gld.vector_d1 = std::gcd(tunable.vector_c, 4 * int(4 / data_byte)); // max: dwordx4 access, calculate how many PIXELs accessed at once;

        ctrl_in_gld.use_flag = 1;
        ctrl_wei_gld.use_flag = 0;

    }
    // Nested-class should utilize same instruction sequence container
    struct global_load_in_t{
        global_load_in_t(igemm_fwd_gtc_nchwc_t* generator_):generator(generator_){}
        int get_issues(){
            

        }
        std::string operator()(){}

    private:
        igemm_fwd_gtc_nchwc_t* generator;
    };
    struct global_load_wei_t{
        global_load_wei_t(igemm_fwd_gtc_nchwc_t* generator_):generator(generator_){}
    private:
        igemm_fwd_gtc_nchwc_t* generator;
    };
    struct shared_store_in_t{};
    struct shared_store_wei_t{};
    

    int in_thread_copy_ndim;
    int wei_thread_copy_ndim;
    int coalescing_store_groups;


    igemm_dotx_mapping_t dotx_mapping;
    igemm_coalescing_store_dotx_t coalescing_store;
    igemm_gtc_tunable_t tunable;
};