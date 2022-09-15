# define LANEGROUP_SIZE 8
struct ctrl_dotx_mapping_t{
/*
    dlops dotx mapping
    following dpp8's term, each lanegroup is a 8x8 matrix, and not support interleave inside lanegroup
    C matrix:

    m_dim   | thread_length                | cluster_length
    --------+------------------------------+------------------------------+
    level_0 | lanegroup_m_per_thread(), 8x | lanegroup_m_per_cluster(), 1 | -> lanegroup_tile_m
    level_1 | 1                            | lanegroup_m_per_wave()       |
    level_2 | 1                            | waves_per_m()                |
    level_3 | lanegroup_repeat_m           | 1                            |

    n_dim   | thread_length                | cluster_length
    --------+------------------------------+------------------------------+
    level_0 | lanegroup_n_per_thread()     | lanegroup_n_per_cluster(), 8 | -> lanegroup_tile_n
    level_1 | 1                            | lanegroup_n_per_wave()       |
    level_2 | 1                            | waves_per_n()                |
    level_3 | lanegroup_repeat_n           | 1                            |

    -----------------------------------------------------------------------------------

    A/B matrix:

    m_dim   | thread_length                | cluster_length
    --------+------------------------------+------------------------------+
    level_0 | thread_m()                   | lanegroup_size_m(), 8        |
    level_1 | 1                            | lanegroup_m_per_wave()       | same as C
    level_2 | 1                            | waves_per_m()                | same as C
    level_3 | lanegroup_repeat_m           | 1                            | same as C

    n_dim   | thread_length                | cluster_length
    --------+------------------------------+------------------------------+
    level_0 | thread_n()                   | lanegroup_size_n(), 8        |
    level_1 | 1                            | lanegroup_n_per_wave()       | same as C
    level_2 | 1                            | waves_per_n()                | same as C
    level_3 | lanegroup_repeat_n           | 1                            | same as C

    k_dim   | thread_length                | cluster_length
    --------+------------------------------+------------------------------+
    level_0 | lanegroup_k_per_thread()     | 1                            |

    @note:
    we can still pack k inside LDS
    e.g.
    v_dot2c_f32_f16, lanegroup_k_per_thread()=2
    but we may prefer locate 8 continus k in LDS, to form a single ds_read_b128
    we may still have thread_m() = 1, which means we only have single m, but intead 8 of k

*/
};

ctrl_dotx_mapping_t get_ctrl_dotx_mapping_from_lanegroup_tile(){


}

struct igemm_dotx_mapping_t{};
