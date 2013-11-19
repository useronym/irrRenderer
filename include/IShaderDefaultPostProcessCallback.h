// This file is part of the "irrRenderer".
// For conditions of distribution and use, see copyright notice in irrRenderer.h

#ifndef ISHADERDEFAULTPOSTPROCESSCALLBACK_H
#define ISHADERDEFAULTPOSTPROCESSCALLBACK_H

#include <irrlicht.h>

namespace irr
{
namespace video
{

class IShaderDefaultPostProcessCallback : public irr::video::IShaderConstantSetCallBack
{
    public:
        virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData);
};

}
}

#endif // ISHADERDEFAULTPOSTPROCESSCALLBACK_H
