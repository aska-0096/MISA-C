#ifndef DOTX_MAPPING_H
#define DOTX_MAPPING_H

#include <cassert>

#include "../../gxco/gxco.h"
#include "dotx.h"
#define LANEGROUP_SIZE 8
#define DOTX_USE_VOP3P true

struct ctrl_dotx_mapping_t
{
    /*
        dlops dotx mapping
        following dpp8's term, each lanegroup is a 8x8 matrix, and not support interleave inside
       lanegroup C matrix:

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
    ctrl_dotx_mapping_t(uint32_t macro_tile_m_,
                        uint32_t macro_tile_n_,
                        uint32_t lanegroup_tile_m_,
                        uint32_t lanegroup_tile_n_,
                        uint32_t lanegroup_thrd_m_,
                        uint32_t lanegroup_thrd_n_,
                        uint32_t lanegroup_wave_m_,
                        uint32_t lanegroup_wave_n_,
                        uint32_t waves_,
                        uint32_t lanegroup_repeat_m_,
                        uint32_t lanegroup_repeat_n_,
                        const std::string& inst_dotx_)
        : macro_tile_m(macro_tile_m_),
          macro_tile_n(macro_tile_m_),
          lanegroup_tile_m(lanegroup_tile_m_),
          lanegroup_tile_n(lanegroup_tile_n_),
          lanegroup_thrd_m(lanegroup_thrd_m_),
          lanegroup_thrd_n(lanegroup_thrd_n_),
          lanegroup_wave_m(lanegroup_wave_m_),
          lanegroup_wave_n(lanegroup_wave_n_),
          waves(waves_),
          lanegroup_repeat_m(lanegroup_repeat_m_),
          lanegroup_repeat_n(lanegroup_repeat_n_),
          inst_dotx(inst_dotx_)
    {
        assert((lanegroup_tile_m % LANEGROUP_SIZE == 0) &&
               (lanegroup_tile_n % LANEGROUP_SIZE == 0));
        assert(wave_size() != 32 && wave_size() != 64);
        assert(macro_tile_m % (lanegroup_repeat_m * lanegroup_tile_m * lanegroup_wave_m) == 0);
        assert(macro_tile_n % (lanegroup_repeat_n * lanegroup_tile_n * lanegroup_wave_n) == 0);

        macro_tile_validate();
        wave_tile_validate();
    }
    uint32_t wave_size() { return lanegroup_wave_m * lanegroup_wave_n * LANEGROUP_SIZE; }
    uint32_t block_size() { return waves * wave_size(); }

    uint32_t lanegroup_m_per_thread()
    {
        /* [within thread] need be 8x */
        return lanegroup_thrd_m;
    }

    uint32_t lanegroup_n_per_thread()
    {
        /* [within thread] */
        return lanegroup_thrd_n;
    }

    uint32_t lanegroup_m_per_cluster() { return lanegroup_tile_m / lanegroup_thrd_m; }

    uint32_t lanegroup_n_per_cluster() { return lanegroup_tile_n / lanegroup_thrd_n; }

    uint32_t lanegroup_m_per_wave()
    {
        /* how many lanegroups to form a single wave, in m direction */
        return lanegroup_wave_m;
    }

    uint32_t lanegroup_n_per_wave()
    {
        /* how many lanegroups to form a single wave, in m direction */
        return lanegroup_wave_n;
    }

    uint32_t waves_per_m()
    {
        return macro_tile_m / (lanegroup_repeat_m * lanegroup_tile_m * lanegroup_wave_m);
    }

    uint32_t waves_per_n()
    {
        return macro_tile_n / (lanegroup_repeat_n * lanegroup_tile_n * lanegroup_wave_n);
    }

    uint32_t lanegroup_size_m()
    {
        if(!dotx_support_dpp8(inst_dotx))
            return lanegroup_tile_m / lanegroup_thrd_m;
        else
            return LANEGROUP_SIZE;
    }

    uint32_t lanegroup_size_n()
    {
        if(!dotx_support_dpp8(inst_dotx))
            return lanegroup_tile_n / lanegroup_thrd_n;
        else
            return LANEGROUP_SIZE;
    }

    uint32_t thread_m()
    {
        if(!dotx_support_dpp8(inst_dotx))
            return lanegroup_thrd_m;
        else
            return lanegroup_tile_m / lanegroup_size_m();
    }

    uint32_t thread_n()
    {
        if(!dotx_support_dpp8(inst_dotx))
            return lanegroup_thrd_n;
        else
            return lanegroup_tile_n / lanegroup_size_n();
    }

    void macro_tile_validate()
    {
        assert(macro_tile_m == lanegroup_m_per_thread() * lanegroup_m_per_cluster() *
                                   lanegroup_m_per_wave() * waves_per_m() * lanegroup_repeat_m);
        assert(macro_tile_n == lanegroup_n_per_thread() * lanegroup_n_per_cluster() *
                                   lanegroup_n_per_wave() * waves_per_n() * lanegroup_repeat_n);
        assert(macro_tile_m * macro_tile_n == block_size() * lanegroup_m_per_thread() *
                                                  lanegroup_repeat_m * lanegroup_n_per_thread() *
                                                  lanegroup_repeat_n);
        assert(macro_tile_m == thread_m() * lanegroup_size_m() * lanegroup_m_per_wave() *
                                   waves_per_m() * lanegroup_repeat_m);
        assert(macro_tile_n == thread_n() * lanegroup_size_n() * lanegroup_n_per_wave() *
                                   waves_per_n() * lanegroup_repeat_n);
    }

    void wave_tile_validate()
    {
        assert(lanegroup_m_per_cluster() * lanegroup_n_per_cluster() == LANEGROUP_SIZE);
        uint32_t wave_tile_m =
            lanegroup_m_per_thread() * lanegroup_m_per_cluster() * lanegroup_m_per_wave();
        uint32_t wave_tile_n =
            lanegroup_n_per_thread() * lanegroup_n_per_cluster() * lanegroup_n_per_wave();
        assert(wave_tile_m == macro_tile_m / (waves_per_m() * lanegroup_repeat_m));
        assert(wave_tile_n == macro_tile_n / (waves_per_n() * lanegroup_repeat_n));
        assert(wave_tile_m == thread_m() * lanegroup_size_m() * lanegroup_m_per_wave());
        assert(wave_tile_n == thread_n() * lanegroup_size_n() * lanegroup_n_per_wave());
    }

    uint32_t macro_tile_m;
    uint32_t macro_tile_n;
    uint32_t lanegroup_tile_m;
    uint32_t lanegroup_tile_n;
    uint32_t lanegroup_thrd_m;
    uint32_t lanegroup_thrd_n;
    uint32_t lanegroup_wave_m;
    uint32_t lanegroup_wave_n;
    uint32_t waves;
    uint32_t lanegroup_repeat_m;
    uint32_t lanegroup_repeat_n;
    std::string inst_dotx;
};

ctrl_dotx_mapping_t get_ctrl_dotx_mapping_from_lanegroup_tile() {}
std::string get_dotx_fma_instruction(const gxco::arch_t& arch, const std::string precision)
{
    std::string arch_str = amdgpu_arch_to_string(arch);
    if(arch_str == "gfx1030")
    {
        if(precision.substr(0, 4) == "fp16")
        {
            if(DOTX_USE_VOP3P)
                return "v_dot2_f32_f16";
            else
                return "v_dot2c_f32_f16";
        }
        else if(precision.substr(0, 4) == "int8")
        {
            if(DOTX_USE_VOP3P)
                return "v_dot4_i32_i8";
            else
                return "v_dot4c_i32_i8";
        }
        else if(precision.substr(0, 4) == "int4")
            return "v_dot8_i32_i4";
        else
        {
            assert(0);
            return "No instruction avaliable";
        }
    }
    else
    {
        assert(0);
        return "No instruction avaliable";
    }
}

struct igemm_dotx_mapping_t
{
    igemm_dotx_mapping_t();
    ctrl_dotx_mapping_t ctrl;
};

#endif // DOTX_MAPPING_H
