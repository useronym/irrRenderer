// This file is part of the "irrRenderer".
// For conditions of distribution and use, see copyright notice in irrRenderer.h

#ifndef E_POSTPROCESSING_EFFECT_H
#define E_POSTPROCESSING_EFFECT_H

namespace irr
{
namespace video
{

enum E_POSTPROCESSING_EFFECT
{
    EPE_ANTIALIASING= 0,
    EPE_BLOOM_PREPASS,
    EPE_BLUR_V,
    EPE_BLUR_H_ADD,
    EPE_FOG,
    EPE_COLD_COLORS,
    EPE_WARM_COLORS,
};

}
}

#endif
