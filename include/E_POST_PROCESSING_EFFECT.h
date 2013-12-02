// This file is part of the "irrRenderer".
// For conditions of distribution and use, see copyright notice in irrRenderer.h

#ifndef E_POSTPROCESSING_EFFECT_H
#define E_POSTPROCESSING_EFFECT_H

namespace irr
{
namespace video
{

//! Enumerators for the included post-processing effects.
enum E_POSTPROCESSING_EFFECT
{
    EPE_ANTIALIASING= 0,

    //! Only let bright areas of the render pass through
    EPE_BLOOM_PREPASS,
    //! Blur render vertically
    EPE_BLUR_V,
    //! Blur render horizontally
    EPE_BLUR_H,
    //! Blur horizontally and add to a supplied texture (on index 0)
    EPE_BLUR_H_ADD,

    //! Pixel fog
    EPE_FOG,
    EPE_COLD_COLORS,
    EPE_WARM_COLORS,
};

}
}

#endif
