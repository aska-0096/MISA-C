#include "../gxco/gxco.h"
#include "igemm_gtc_base.h"
#include "operations/global_memory.h"
#include "operations/dotx_mapping.h"

#define IGEMM_FWD_GTC_NCHWC_DEBUG 0

#define IGEMM_FWD_GTC_NCHWC_INT4_VOP3P_K_PACK 16 // can be 8/16/32
#define IGEMM_FWD_GTC_NCHWC_INT8_VOP3P_K_PACK 4
#define IGEMM_FWD_GTC_NCHWC_FP16_VOP3P_K_PACK 2

#define IGEMM_FWD_GTC_NCHWC_16BIT_SPATIAL_INDEXING 1

std::vector<int> _find_non_1_index_in_list(const std::vector<int>& src){
    std::vector<int> result;
    for(auto& element : src){
        if(element != 1) result.push_back(element);
    }

    return result;
}

int gpr_align(int previous_cnt, int alignment){
    return ((previous_cnt + alignment - 1) / alignment) * alignment;
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
        // Get control blocks ready
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
        
        // Fill up macro, variable, kernel_code container.
        create_kernel_instance();
        // Instance ready for emit call from emitter.
    }

private:
    
    uint32_t get_num_vgpr_global_load_a(){
        thread_lengths tl = get_thread_lengths();
        uint32_t pack_factor = tl.ta_k_vec_c != 1 ? uint32_t(4 / amdgpu_precision_data_byte(tunable.precision)) : 1;
        return tunable.num_global_load_a / pack_factor;
    }

    uint32_t get_num_vgpr_global_load_b(){
        thread_lengths tl = get_thread_lengths();
        uint32_t pack_factor = tl.tb_nb_vec_c != 1 ? uint32_t(4 / amdgpu_precision_data_byte(tunable.precision)) : 1;
        return tunable.num_global_load_b / pack_factor;
    }

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

    uint32_t get_k_pack(){
        thread_lengths tl = get_thread_lengths();
        uint32_t lds_k_pack;
        if (!dotx_support_dpp8(dotx_mapping.ctrl.inst_dotx)){
            if (tunable.precision == "int4")
                lds_k_pack = std::gcd(tl.tb_nb_vec_c, IGEMM_FWD_GTC_NCHWC_INT4_VOP3P_K_PACK);
            else if (tunable.precision == "int8")
                lds_k_pack = std::gcd(tl.tb_nb_vec_c, IGEMM_FWD_GTC_NCHWC_INT8_VOP3P_K_PACK);
            else if (tunable.precision == "fp16")
                lds_k_pack = std::gcd(tl.tb_nb_vec_c, IGEMM_FWD_GTC_NCHWC_FP16_VOP3P_K_PACK);
        }
        else
            lds_k_pack = tl.tb_nb_vec_c;
        return lds_k_pack;
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
    
    void def_alias_karg(){
        GXCO_VAR(k_p_in          ,0);
        GXCO_VAR(k_p_wei         ,8);
        GXCO_VAR(k_p_out         ,16);
        GXCO_VAR(k_tile_hw       ,24);
        GXCO_VAR(k_ntile_hw      ,28);
        GXCO_VAR(k_hi            ,32);
        GXCO_VAR(k_wi            ,36);
        GXCO_VAR(k_n             ,40);
        GXCO_VAR(k_k             ,44);
        GXCO_VAR(k_c             ,48);
        GXCO_VAR(k_group         ,52);
        GXCO_VAR(k_ks            ,56);
        GXCO_VAR(k_ho            ,60);
        GXCO_VAR(k_wo            ,64);
        GXCO_VAR(k_stride_hw       ,68);
        GXCO_VAR(k_dilation_hw     ,72);
        GXCO_VAR(k_pad_hw          ,76);
        GXCO_VAR(k_wei_hw          ,80);
        GXCO_VAR(k_move_slice_k    ,84);

        GXCO_VAR(k_magic_0         ,88);
        GXCO_VAR(k_magic_1         ,92);
        GXCO_VAR(k_magic_2         ,96);
        GXCO_VAR(k_magic_3         ,100);
        GXCO_VAR(k_magic_4         ,104);
        GXCO_VAR(k_magic_5         ,108);
        GXCO_VAR(k_magic_6         ,112);
        GXCO_VAR(k_magic_7         ,116);
        GXCO_VAR(k_shift_pack_0    ,120);
        GXCO_VAR(k_shift_pack_1    ,124);
        GXCO_VAR(k_end             ,128);
    }

    void def_alias_sgpr(){
        thread_lengths tl = get_thread_lengths();
        int sgpr_cnt = 0;
        GXCO_VAR(s_ka                      , sgpr_cnt += 2);
        GXCO_VAR(s_bx                      , sgpr_cnt += 1);
        GXCO_VAR(s_by                      , sgpr_cnt += 1);
        GXCO_VAR(s_p_in                    , sgpr_cnt += 4);
        GXCO_VAR(s_p_wei                   , sgpr_cnt += 4);
        GXCO_VAR(s_p_out                   , sgpr_cnt += 4);
        GXCO_VAR(s_tile_hw                 , sgpr_cnt += 1);
        GXCO_VAR(s_ntile_hw                , sgpr_cnt += 1);
        GXCO_VAR(s_hi                      , sgpr_cnt += 1);
        GXCO_VAR(s_wi                      , sgpr_cnt += 1);
        GXCO_VAR(s_n                       , sgpr_cnt += 1);
        GXCO_VAR(s_k                       , sgpr_cnt += 1);    // this is indeed k_per_group
        GXCO_VAR(s_c                       , sgpr_cnt += 1);    // this is indeed c_per_group
        GXCO_VAR(s_group                   , sgpr_cnt += 1);
        GXCO_VAR(s_gemmk_split             , sgpr_cnt += 1);

        if (tunable.nxe != 0){
            GXCO_VAR(s_ho                      , sgpr_cnt += 1);
            GXCO_VAR(s_wo                      , sgpr_cnt += 1);
            GXCO_VAR(s_stride_hw               , sgpr_cnt += 1);
            GXCO_VAR(s_dilation_hw             , sgpr_cnt += 1);
            GXCO_VAR(s_pad_hw                  , sgpr_cnt += 1);
            GXCO_VAR(s_wei_hw                  , sgpr_cnt += 1);
            GXCO_VAR(s_move_slice_k            , sgpr_cnt += 1);
        }
        sgpr_cnt = gpr_align(sgpr_cnt, 8);
        GXCO_VAR(s_magic_0                 , sgpr_cnt += 1);
        GXCO_VAR(s_magic_1                 , sgpr_cnt += 1);
        GXCO_VAR(s_magic_2                 , sgpr_cnt += 1);
        GXCO_VAR(s_magic_3                 , sgpr_cnt += 1);
        GXCO_VAR(s_magic_4                 , sgpr_cnt += 1);
        GXCO_VAR(s_magic_5                 , sgpr_cnt += 1);
        GXCO_VAR(s_magic_6                 , sgpr_cnt += 1);
        GXCO_VAR(s_magic_7                 , sgpr_cnt += 1);
        GXCO_VAR(s_shift_pack_0            , sgpr_cnt += 1);
        GXCO_VAR(s_shift_pack_1            , sgpr_cnt += 1);

        if (tunable.nxe != 0){
            GXCO_VAR(s_stride_h                , sgpr_cnt += 1);
            GXCO_VAR(s_stride_w                , s_stride_hw.get_value());
            GXCO_VAR(s_dilation_h              , sgpr_cnt += 1);
            GXCO_VAR(s_dilation_w              , s_dilation_hw.get_value());
            GXCO_VAR(s_pad_h                   , sgpr_cnt += 1);
            GXCO_VAR(s_pad_w                   , s_pad_hw.get_value());
            GXCO_VAR(s_y                       , sgpr_cnt += 1);
            GXCO_VAR(s_x                       , s_wei_hw.get_value());
        }
        GXCO_VAR(s_i_tile_h                , sgpr_cnt += 1);
        GXCO_VAR(s_i_tile_w                , sgpr_cnt += 1);
        GXCO_VAR(s_tile_h                  , sgpr_cnt += 1);
        GXCO_VAR(s_tile_w                  , s_tile_hw.get_value());
        GXCO_VAR(s_ntile_h                 , sgpr_cnt += 1);
        GXCO_VAR(s_ntile_w                 , s_ntile_hw.get_value());
        GXCO_VAR(s_sps_hi                  , sgpr_cnt += 1);
        GXCO_VAR(s_sps_wi                  , sgpr_cnt += 1);
        GXCO_VAR(s_tile_os_hi              , sgpr_cnt += 1);
        GXCO_VAR(s_tile_os_wi              , sgpr_cnt += 1);

        if (tunable.nxe != 0){
            GXCO_VAR(s_sps_ho                  , sgpr_cnt += 1);
            GXCO_VAR(s_sps_wo                  , sgpr_cnt += 1);
            GXCO_VAR(s_sps_py                  , sgpr_cnt += 1);
            GXCO_VAR(s_sps_px                  , sgpr_cnt += 1);
            GXCO_VAR(s_tile_os_ho              , sgpr_cnt += 1);
            GXCO_VAR(s_tile_os_wo              , sgpr_cnt += 1);
            GXCO_VAR(s_in_stride_c             , sgpr_cnt += 1);
        }
        GXCO_VAR(s_in_stride_c             , sgpr_cnt += 1);
        GXCO_VAR(s_in_stride_hi            , sgpr_cnt += 1);
        GXCO_VAR(s_in_stride_n             , sgpr_cnt += 1);

        if (tl.tb_nb0 != 1)
            GXCO_VAR(s_in_stride_nb0           , sgpr_cnt += 1);
        if(tunable.tensor_layout == "nchwc_kcyxc")
            GXCO_VAR(s_wei_stride_k            , sgpr_cnt += 1);
        else
            GXCO_VAR(s_wei_stride_x            , sgpr_cnt += 1);
        
        GXCO_VAR(s_out_stride_k            , sgpr_cnt += 1);
        if (coalescing_store.need_vector_m_inside_fold_m())
            GXCO_VAR(s_out_stride_vector_k     , s_in_stride_c.get_value());
        GXCO_VAR(s_out_stride_ho           , sgpr_cnt += 1);
        GXCO_VAR(s_out_stride_n            , sgpr_cnt += 1);

        GXCO_VAR(s_block_gtc_ig            , sgpr_cnt += 1);
        GXCO_VAR(s_block_gtc_ik            , sgpr_cnt += 1);
        GXCO_VAR(s_block_gtc_inb           , sgpr_cnt += 1);
        
        GXCO_VAR(s_move_slice_k_stride_gemm_k  , sgpr_cnt += 1);
        GXCO_VAR(s_move_slice_k_stride_c   , sgpr_cnt += 1);
        GXCO_VAR(s_knum                    , 3);

        GXCO_VAR(s_dim_br                  , sgpr_cnt += 1);
        GXCO_VAR(s_dim_mp                  , sgpr_cnt += 1);
        GXCO_VAR(s_dim_mr                  , sgpr_cnt += 1);
        GXCO_VAR(s_dim_np                  , sgpr_cnt += 1);
        GXCO_VAR(s_dim_nr                  , sgpr_cnt += 1);

        if (tunable.gemm_k_global_split)
            GXCO_VAR(s_gemm_k_diff_c           , s_group.get_value());
        
        if (tunable.nxe != 0){
            GXCO_VAR(s_move_slice_k_y          , sgpr_cnt += 1);
            GXCO_VAR(s_move_slice_k_x          , sgpr_cnt += 1);
            GXCO_VAR(s_move_slice_k_y_dh       , s_move_slice_k_y.get_value());
            GXCO_VAR(s_move_slice_k_x_dw       , s_move_slice_k_x.get_value());
            GXCO_VAR(s_move_slice_k_c          , s_move_slice_k.get_value());
            GXCO_VAR(s_diff_in_os_acc_c_y_x    , s_block_gtc_ig.get_value());
            GXCO_VAR(s_diff_in_os_ovf_y_acc_c  , 0);
            GXCO_VAR(s_diff_in_os_ovf_x_acc_y  , s_dim_br.get_value());
            GXCO_VAR(s_diff_in_iwi_acc_x       , s_dim_mp.get_value());
            GXCO_VAR(s_diff_in_iwi_ovf_x       , s_dim_np.get_value());
            GXCO_VAR(s_diff_in_ihi_acc_y       , s_pad_w.get_value());
            GXCO_VAR(s_diff_in_ihi_ovf_y       , s_pad_h.get_value());
            GXCO_VAR(s_y_x_c                   , s_dim_nr.get_value());
        }
        else
            GXCO_VAR(s_move_slice_k_acc_c      , sgpr_cnt += 1);
        GXCO_VAR(s_kitr                    , 1);

        if (tunable.precision == "int8")
            GXCO_VAR(s_0xff                    , sgpr_cnt += 1);
        GXCO_VAR(s_0xffff                  , sgpr_cnt += 1);

        if (tunable.tensor_a_pass_through)
            // need s precache
            // in_npc = ((ta_ce1 // k_pack) - 2) if ((ta_ce1 // k_pack) - 2 > 0 ) else 0
            GXCO_VAR(s_in_c_itr                , 2);
        else
            GXCO_VAR(s_in_offset               , sgpr_cnt += 1);
        
        if (tunable.precache_soffset && (tunable.tensor_layout == "nchwc_kcyxc")){
            // cyxkc not need s_offset
            // TODO get_macro_global_load()
            // m_wei_2d_global_load, m_in_2d_global_load         = get_macro_global_load()
            // uint32_t wei_npc = m_wei_2d_global_load.get_num_precache_soffset()
            GXCO_VAR(s_wei_offset             , sgpr_cnt += wei_npc);
        }
        if (tunable.gemm_k_global_split){
            GXCO_VAR(s_block_gtc_ic           , sgpr_cnt += 1); // add c-split
            GXCO_VAR(s_sub_c                  , sgpr_cnt += 1);
        }
        
        sgpr_cnt = gpr_align(sgpr_cnt, 2);
        GXCO_VAR(s_tmp                    , sgpr_cnt += 6);

        if (IGEMM_FWD_GTC_NCHWC_DEBUG == 1){
            sgpr_cnt = gpr_align(sgpr_cnt, 2);
            GXCO_VAR(s_dbg                    , sgpr_cnt += 4);
        }

        GXCO_VAR(s_x_dilation_w           , s_tile_os_hi.get_value());
        GXCO_VAR(s_y_dilation_h           , s_tile_os_wi.get_value());

        GXCO_VAR(s_end                    , sgpr_cnt);
    }

    void def_alias_vgpr(){
        thread_lengths tl = get_thread_lengths();
        cluster_lengths cl = get_cluster_lengths();

        uint32_t nb_per_thread = tb_nb0;
        uint32_t nk_per_thread = ta_k_vec_c;
        assert(nb_per_thread <= 16); //"we pack flag into single vgpr"

        uint32_t k_pack = get_k_pack();
        uint32_t share_load_packed  = k_pack;

        bool is_vgpr_acc_c = (tunable.fma_type != IGEMM_GTC_TUNABLE_FMA_TYPE_XDLOPS);
        float data_byte                   = amdgpu_precision_data_byte(tunable.precision);
        uint32_t num_vgpr_global_load_a      = get_num_vgpr_global_load_a();
        uint32_t num_vgpr_global_load_b      = get_num_vgpr_global_load_b();

        uint32_t share_load_packed_vgpr      = share_load_packed / uint32_t(4 / data_byte);

        uint32_t num_vgpr_acc_a              = (!tunable.tensor_a_pass_through) ? share_load_packed_vgpr * tunable.num_vgpr_accumulate_a : 0;
        uint32_t num_vgpr_acc_b              = (!tunable.tensor_b_pass_through) ? share_load_packed_vgpr * tunable.num_vgpr_accumulate_b : 0;

        //printf("share_load_packed_vgpr:%d, tunable.num_vgpr_accumulate_b:%d, num_vgpr_acc_b:%d", share_load_packed_vgpr, tunable.num_vgpr_accumulate_b, num_vgpr_acc_b);
        uint32_t vgpr_cnt = 0;
        if (is_vgpr_acc_c){
            GXCO_VAR(v_c            , vgpr_cnt += (tunable.num_vgpr_accumulate_c+1));
            uint32_t v_c_num                 = vgpr_cnt;
        }
        else{
            uint32_t v_c_resuable_num        = num_vgpr_acc_a + num_vgpr_acc_b + 
                                               num_vgpr_global_load_a + num_vgpr_global_load_b + 
                                               3 * nb_per_thread + 6;      // from v_sst_a_os to v_co_sst
            //v_c_coalescing_num      = tunable.num_agpr_accumulate_c / coalescing_store_groups
            uint32_t v_c_coalescing_num      = coalescing_store.get_vgpr_usage();
            int v_c_needed              = (v_c_coalescing_num - v_c_resuable_num) > 0 ? (v_c_coalescing_num - v_c_resuable_num) : 0;

            v_c_needed              = v_c_needed > 0 ? v_c_needed : 0;  // let at least 0
            GXCO_VAR(v_c            , vgpr_cnt += v_c_needed); //f"coalescing:{v_c_coalescing_num}, needed:{v_c_needed}, resuable:{v_c_resuable_num}"
        }
        if (!tunable.tensor_a_pass_through)
            GXCO_VAR(v_a               , vgpr_cnt += (num_vgpr_acc_a+1));
        if (!tunable.tensor_b_pass_through)
            GXCO_VAR(v_b               , vgpr_cnt += num_vgpr_acc_b);
        GXCO_VAR(v_gld_a           , vgpr_cnt += num_vgpr_global_load_a);
        if (tunable.global_prefetch_a_num == 2)
            GXCO_VAR(v_gld_a_gpf       , vgpr_cnt += num_vgpr_global_load_a);
        GXCO_VAR(v_gld_b           , vgpr_cnt += num_vgpr_global_load_b);
        if (tunable.global_prefetch_b_num == 2)
            GXCO_VAR(v_gld_b_gpf       , vgpr_cnt += num_vgpr_global_load_b);
        if (!tunable.tensor_a_pass_through){
            GXCO_VAR(v_sst_a_os        , vgpr_cnt += 1);
            GXCO_VAR(v_sld_a_os        , vgpr_cnt += 1);
        }
        if (!tunable.tensor_b_pass_through){
            GXCO_VAR(v_sst_b_os        , vgpr_cnt += 1);
            GXCO_VAR(v_sld_b_os        , vgpr_cnt += 1);
        }

        GXCO_VAR(v_in_os           , vgpr_cnt += nb_per_thread);
        if (IGEMM_FWD_GTC_NCHWC_16BIT_SPATIAL_INDEXING)
            GXCO_VAR(v_in_i_hw_list    , vgpr_cnt += nb_per_thread);
        else{
            GXCO_VAR(v_in_ihi_list     , vgpr_cnt += nb_per_thread);
            GXCO_VAR(v_in_iwi_list     , vgpr_cnt += nb_per_thread);
        }

        GXCO_VAR(v_in_flag         , vgpr_cnt += nb_per_thread);
        GXCO_VAR(v_in_flag_n       , vgpr_cnt += 1);      // bfe this!, lo 16bit is flag_n

        GXCO_VAR(v_wei_os          , vgpr_cnt += 1);
        GXCO_VAR(v_out_os          , vgpr_cnt += 1);

        if (tunable.tensor_a_pass_through)
            GXCO_VAR(v_gtc_ic_a        ,v_gld_a.get_value());
        if (tunable.tensor_b_pass_through)
            GXCO_VAR(v_gtc_ic_b        ,v_gld_b.get_value());
        if (!(tunable.tensor_a_pass_through && tunable.tensor_b_pass_through))
            GXCO_VAR(v_gtc_ic          ,vgpr_cnt += 1);

        assert(!tunable.tensor_b_pass_through);
        GXCO_VAR(v_gtc_iec         ,vgpr_cnt += 1);
        GXCO_VAR(v_gtc_iy          ,vgpr_cnt += 1);
        GXCO_VAR(v_gtc_ix          ,vgpr_cnt += 1);
        GXCO_VAR(v_in_inb          ,vgpr_cnt += 1);
        GXCO_VAR(v_in_in           ,vgpr_cnt += 1);
        GXCO_VAR(v_wei_ik          ,vgpr_cnt += 1);

        GXCO_VAR(v_co_sst          ,v_in_in.get_value());
        GXCO_VAR(v_co_sld          ,vgpr_cnt += 1);

        GXCO_VAR(v_out_flag        ,v_wei_ik.get_value());
        GXCO_VAR(v_out_inb         ,v_in_inb.get_value());
        GXCO_VAR(v_out_ik          ,v_in_inb.get_value());

        GXCO_VAR(v_gemm_in         ,vgpr_cnt += 1);
        GXCO_VAR(v_gemm_im         ,vgpr_cnt += 1);
        GXCO_VAR(v_co_sub_m_index  ,v_gemm_im.get_value());
        GXCO_VAR(v_co_sub_n_index  ,v_gemm_in.get_value());
        GXCO_VAR(v_out_in          ,v_gemm_in.get_value());
            
        GXCO_VAR(v_coalescing_store_index ,v_gemm_in.get_value());
        vgpr_cnt = gpr_align(vgpr_cnt, 2);    
        GXCO_VAR(v_tmp             ,vgpr_cnt += 6);
                
        if (IGEMM_FWD_GTC_NCHWC_DEBUG == 1){
            vgpr_cnt = gpr_align(vgpr_cnt, 2);
            GXCO_VAR(v_dbg            ,vgpr_cnt += 2);
        }
        uint32_t total_vgpr             = sgpr_cnt;
        uint32_t accum_start            = 0;
        if (tunable.fma_type == IGEMM_GTC_TUNABLE_FMA_TYPE_XDLOPS){
            if (ti.arch == gxco::GFX90A){
                total_vgpr          = (total_vgpr + 3) / 4 * 4; // round to multiply of 4
                accum_start    = total_vgpr;
                total_vgpr     = total_vgpr + tunable.num_agpr_accumulate_c;
            }
            else
                // if xdlops agpr is larger than vgpr usage, must change vgpr count to agpr
                total_vgpr          = total_vgpr > tunable.num_agpr_accumulate_c ? total_vgpr : tunable.num_agpr_accumulate_c;
        }
        GXCO_VAR(v_end          ,total_vgpr);
    }

    void create_kernel_instance(){
        // Fill up macro container
        /* macro method here */
        // Fill up variable container
        def_alias_karg();
        def_alias_sgpr();
        def_alias_vgpr();
        /** 
         * Fill up kernel instruction container
         * call control blocks
         * @section prologue
         *     @subsection Load kernel argumens
         *     @subsection (debug mode tools)
         *     @subsection prepare parameters for input data loading later, e.g. strides etc. for thread mapping
         *     @subsection Global load weight and input tensor by call correspond control blocks
         *     @subsection LDS store weight and input tensor by call correspond control blocks
         *     @subsection prepare parameters for output data storing later e.g. strides etc. for thread mapping
         * @section fma_main_loop
         *     @subsection Call selected math type control block
         * @section epilogue
         *     @subsection Call coalescing store control block
         */

        global_load_wei = igemm_global_load_wei_t(get_variables());
        instruction_sequence += global_load_wei();

        
    }
    
    // Common Parameters
    int in_thread_copy_ndim;
    int wei_thread_copy_ndim;
    int coalescing_store_groups;

    // Control Blocks
    igemm_karg_load_t karg_load;
    igemm_input_mapping_param_t input_mapping_param;
    igemm_weight_mapping_param_t weight_mapping_param;
    igemm_global_load_wei_t global_load_wei;
    igemm_global_load_in_t global_load_in;
    igemm_shared_store_wei_t shared_store_wei;
    igemm_shared_store_in_t shared_store_in;
    igemm_output_mapping_param_t output_mapping_param;

    igemm_dotx_mapping_t dotx_mapping;

    igemm_coalescing_store_dotx_t coalescing_store;
    
    igemm_gtc_tunable_t tunable;
};