#ifndef GLOBAL_MEMORY_H
#define GLOBAL_MEMORY_H
struct ctrl_2d_global_load_t
{
    int length_d0         = 1; // if d0 is 1, it is indeed 1d access
    int length_d1         = 1;
    int vector_d1         = 1;
    std::string precision = "fp32"; // 'fp32', 'fp16', ...
    int src_order         = 0;      // 0-d0xd1, 1-d1xd0
    int dst_order         = 0;      // 0-d0xd1, 1-d1xd0
    int use_flag          = 0;
    int flag_on_d0        = 0;
    int flag_on_d1        = 0;
    int bfe_flag          = 0;
    int precache_ptn      = 0; // 0: d0 use sgpr precache, d1 use sgpr precache
                               // 1: d0 use sgpr precache, d1 use vgpr precache
                               // 2: d0 use sgpr precache, d1 use immidiate value
                               // 3: d0 use vgpr precache, d1 use sgpr precache
                               // 4: d0 use vgpr precache, d1 use vgpr precache
                               // 5: d0 use vgpr precache, d1 use immidiate value
                               // 4: .... maybe consider not using precache?
    int flag_merge_v = 0;      // when flag on v_offset, flag and multiple load, or flag per load
    int dim_conti_flag =
        0; // when dim flag is set, means length d0 is contiguous, and sgpr offset will not be used
    int arch_name        = AMDGPU_ARCH_GFX908;
    int workgroup_length = 1;
};
class macro_igemm_2d_global_load_t
{
};

class igemm_global_load_wei_t
{
};
class igemm_global_load_in_t
{
};

#endif // GLOBAL_MEMORY_H
