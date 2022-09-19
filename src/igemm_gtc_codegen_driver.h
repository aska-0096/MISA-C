#include "codegen/igemm/igemm_fwd_nchwc.h"
gxco::generator_t igemm_kernel_gen(const std::vector<igemm_gtc_tunable_t>& tunables,
                                   const gxco::target_info_t& ti,
                                   gxco::emitter_t& emitter)
{
    auto direction     = tunables[0].direction;
    auto tensor_layout = tunables[0].tensor_layout;

    if(direction == "fwd" && tensor_layout == "nchwc")
    {
        return igemm_fwd_gtc_nchwc_t(ti, emitter, tunables);
    }
}