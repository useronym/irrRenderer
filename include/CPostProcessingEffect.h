#ifndef CPOSTPROCESSINGEFFECT_H
#define CPOSTPROCESSINGEFFECT_H

#include <irrlicht.h>


namespace irr
{
namespace video
{

class CPostProcessingEffect
{
    public:
        CPostProcessingEffect(irr::video::E_MATERIAL_TYPE &type, irr::video::IShaderConstantSetCallBack* callback= 0);
        ~CPostProcessingEffect();

        void addTextureToShader(irr::video::ITexture* tex);
        irr::video::ITexture* getTextureToPass(irr::u32 index);
        irr::u32 getTextureToPassCount();
        irr::video::E_MATERIAL_TYPE& getMaterialType();
        irr::video::IShaderConstantSetCallBack* getCallback();
        void setActive(bool active);
        bool isActive();

    private:
        bool Active;
        irr::core::array<irr::video::ITexture*> TexturesToPass;
        irr::video::E_MATERIAL_TYPE MaterialType;
        irr::video::IShaderConstantSetCallBack* Callback;
};

}
}

#endif // CPOSTPROCESSINGEFFECT_H
