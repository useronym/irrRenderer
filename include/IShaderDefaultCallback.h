// This file is part of the "irrRenderer".
// For conditions of distribution and use, see copyright notice in irrRenderer.h

#ifndef ISHADERDEFAULTCALLBACK_H
#define ISHADERDEFAULTCALLBACK_H

#include <irrlicht.h>

namespace irr{
namespace video{

class IShaderDefaultCallback : public irr::video::IShaderConstantSetCallBack
{
    virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData);

    virtual void OnSetMaterial(const irr::video::SMaterial &material);

private:
    irr::f32 Lighting;
    irr::f32 Param1, Param2;
};

}
}

#endif // ISHADERDEFAULTCALLBACK_H
