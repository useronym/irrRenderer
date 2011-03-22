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
