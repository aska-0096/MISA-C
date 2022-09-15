#define IGEMM_GTC_FEAT_ALLOW_LDS_REORDER  0
#define IGEMM_GTC_FEAT_PRECACHE_SOFFSET  1
#define IGEMM_GTC_FEAT_LOCAL_PREFETCH  1
#define IGEMM_GTC_FEAT_FMA_INTERLEAVE 1
#define IGEMM_GTC_FEAT_MAGIC_DIVISION  1
#define IGEMM_GTC_FEAT_SOURCE_ACCESS_ENCODING_KERNEL_NAME 0

// IGEMM_GTC_TENSOR_LAYOUT_NCHW = ((1 << 4) | 0)
// IGEMM_GTC_TENSOR_LAYOUT_NHWC = ((1 << 4) | 1)
// IGEMM_GTC_TENSOR_LAYOUT_CNHW = ((1 << 4) | 2)

#define IGEMM_GTC_TUNABLE_FMA_TYPE_MAC               "mac"
#define IGEMM_GTC_TUNABLE_FMA_TYPE_DLOPS             "dlops"
#define IGEMM_GTC_TUNABLE_FMA_TYPE_WMMA              "wmma"
#define IGEMM_GTC_TUNABLE_FMA_TYPE_XDLOPS            "xdlops"


struct lanegroup_mapping_parameter_t{
    int tile_m;
    int tile_n;
    int wave_m;
    int wave_n;
    int repeat_m;
    int repeat_n;
};

struct fma_mapping_parameter_t{
    int gemm_m_per_thread;
    int gemm_n_per_thread;
    int gemm_m_level0_cluster;
    int gemm_n_level0_cluster;
    int gemm_m_level1_cluster;
    int gemm_n_level1_cluster;
};

struct xdlops_mapping_parameter_t{
    int wave_tile_m;
    int wave_tile_n;
    int wave_wave_m;
    int wave_wave_n;
    int wave_repeat_m;
    int wave_repeat_n;
    int wave_tile_k;
};

// @brief generic tensor contraction
struct igemm_gtc_tunable_parameter_t{
    // Convolution descriptor
    std::string tensor_layout;
    std::string fma_type;
    std::string direction;
    std::string precision;
    int vector_c;
    int wavefront_size;
    int cumode;
    // Macro tile
    int gemm_m_per_block;
    int gemm_n_per_block;
    int gemm_k_per_block;
    // Micro tile, thread mapping
    lanegroup_mapping_parameter_t lanegroup;
    fma_mapping_parameter_t fma;
    xdlops_mapping_parameter_t xdlops;
    std::vector<int> tensor_a_thread_lengths; 
    std::vector<int> tensor_a_cluster_lengths;
    std::vector<int> tensor_b_thread_lengths; 
    std::vector<int> tensor_b_cluster_lengths;
    // Optimization Option List -- LDS pass
    bool tensor_a_pass_through;
    bool tensor_b_pass_through;
    bool nxb;
    bool nxe;
    bool multihead;
    int gemm_k_global_split;
    bool allow_lds_reorder;
    bool precache_soffset;
    int source_access_order;
    bool gemm_m_unmerge_cluster;
    bool gemm_n_unmerge_cluster;
    bool gemm_k_unmerge_cluster;
    bool vector_store;
    bool merge_e;
};