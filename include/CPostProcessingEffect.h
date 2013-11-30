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
        /**
         * Constructor, only used internally.
         * @param type material for this effect
         * @param callback pointer to a callback which should be used, if any
         */
        CPostProcessingEffect(irr::video::E_MATERIAL_TYPE &type, irr::video::IShaderConstantSetCallBack* callback= 0);
        /**
         * Destructor.
         */
        ~CPostProcessingEffect();

        /**
         * Sets the chain this effect should belong to.
         * @param chain the chain the effect will belong to
         */
        void setChain(irr::video::CPostProcessingEffectChain* chain);
        /**
         * Returns a pointer to the chain this effect belongs to. Note: Effects which don't have a chain explicitly set in code belong to a "root" effect chain.
         * @return The chain this effect belongs to.
         */
        irr::video::CPostProcessingEffectChain* getChain() const;
        /**
         * Completely removes this effect.
         */
        void remove();
        /**
         * Sets whether this effect should be currently active.
         * @param active true or false
         */
        void setActive(bool active);
        /**
         * Returns whether is this effect active.
         * @return True if active.
         */
        bool isActive() const;

        /**
         * Adds a new texture which should be passed to the shader of this effect.
         * @param tex texture to pass to the shader
         */
        void addTextureToShader(irr::video::ITexture* tex);
        /**
         * Gets the texture which should be passed to this shader on the given index.
         * @param index index of the texture, starting from 0
         * @return The texture on the given index.
         */
        irr::video::ITexture* getTextureToPass(irr::u32 index) const;
        /**
         * Gets the amount of textures that should be passed to this shader.
         * @return How many textures this shader requested should be passed to it.
         */
        irr::u32 getTextureToPassCount() const;
        /**
         * Gets the material of this effect's shader.
         * @return The material of this effect.
         */
        const irr::video::E_MATERIAL_TYPE& getMaterialType() const;
        /**
         * Gets the callback this effect's shader uses, if any.
         * @return The shader callback, 0 if none.
         */
        irr::video::IShaderConstantSetCallBack* getCallback() const;

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
