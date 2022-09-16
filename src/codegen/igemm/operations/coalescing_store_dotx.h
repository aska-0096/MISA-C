#include <string>
#include <cassert>
#include "dotx_mapping.h"

struct ctrl_coalescing_store_dotx_t{
    ctrl_dotx_mapping_t cdm;                    // ctrl_dotx_mapping_t
    int coalescing_groups = 1;
    int block_size = 256;
    int vector_store_m = 1;             // global vector store in m/n
    int vector_store_n = 1;            // ... m, n can't be non-1 at the same time
    int vector_fold_m = 1;              // due to vector store, we might want to fold m/n
    int vector_fold_n = 1;              // ... while calculating m/n global index    -> ignore now
    std::string precision = "fp16";            // dotx only support fp16 & int8
    bool gemm_k_global_split = false;
    bool feat_vgpr_collapse = true;
    bool feat_co_m_flag_check = false;   // custom flag check, not using internal check
    // co_m_update_os_functor = None  // update offset based on current i_m. otherwise use sgpr to update offset
    // co_m_flag_check_start_functor = None
    // co_m_flag_check_reset_functor = None
    // div_v_const_func = None
    // div_rem_v_const_func = None
    // mul_vi_func = None
    // mul_si_func = None
};
struct igemm_coalescing_store_dotx_t{

    bool need_vector_m_inside_fold_m(){
        if (ctrl.vector_fold_m != 1)
            if (ctrl.vector_fold_m > ctrl.vector_store_m)
            {
                assert(ctrl.vector_fold_m % ctrl.vector_store_m == 0);
                if (ctrl.cdm.block_size() >= ctrl.cdm.macro_tile_n)
                    return true;
            }
        return false;
    }

    ctrl_coalescing_store_dotx_t ctrl;
};