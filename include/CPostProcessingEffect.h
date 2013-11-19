// This file is part of the "irrRenderer".
// For conditions of distribution and use, see copyright notice in irrRenderer.h

#ifndef CPOSTPROCESSINGEFFECT_H
#define CPOSTPROCESSINGEFFECT_H

#include <irrlicht.h>


namespace irr
{
namespace video
{

class CPostProcessingEffectChain;

class CPostProcessingEffect
{
    public:
        CPostProcessingEffect(irr::video::E_MATERIAL_TYPE &type, irr::video::IShaderConstantSetCallBack* callback= 0);
        ~CPostProcessingEffect();

        void setChain(irr::video::CPostProcessingEffectChain* chain);
        irr::video::CPostProcessingEffectChain* getChain() const;

        void remove();

        void addTextureToShader(irr::video::ITexture* tex);
        irr::video::ITexture* getTextureToPass(irr::u32 index) const;
        irr::u32 getTextureToPassCount() const;
        const irr::video::E_MATERIAL_TYPE& getMaterialType() const;
        irr::video::IShaderConstantSetCallBack* getCallback() const;
        void setActive(bool active);
        bool isActive() const;

    private:
        irr::video::CPostProcessingEffectChain* Chain;

        bool Active;
        irr::core::array<irr::video::ITexture*> TexturesToPass;
        irr::video::E_MATERIAL_TYPE MaterialType;
        irr::video::IShaderConstantSetCallBack* Callback;
};

}
}

#endif // CPOSTPROCESSINGEFFECT_H
