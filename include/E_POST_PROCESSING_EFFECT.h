#ifndef E_POSTPROCESSING_EFFECT_H
#define E_POSTPROCESSING_EFFECT_H

namespace irr
{
namespace video
{

enum E_POSTPROCESSING_EFFECT
{
    EPE_ANTIALIASING= 0,
    EPE_BLOOM,
    EPE_BLOOM_LQ,
    EPE_CONTRAST,
    EPE_COLD_COLORS,
    EPE_WARM_COLORS,
    //HDR effects
    EPE_TONE_MAPPING
};

}
}

#endif
