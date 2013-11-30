// This file is part of the "irrRenderer".
// For conditions of distribution and use, see copyright notice in irrRenderer.h

#ifndef CPOSTPROCESSINGEFFECTCHAIN_H
#define CPOSTPROCESSINGEFFECTCHAIN_H

#include <irrlicht.h>
#include "CPostProcessingEffect.h"
#include "E_POST_PROCESSING_EFFECT.h"
#include "IShaderDefaultPostProcessCallback.h"

namespace irr
{
namespace video
{

class CRenderer;
class SShaderSource;

class CPostProcessingEffectChain
{
    public:
        /**
         * Constructor, only used internally.
         * @param renderer renderer this chain belongs to
         */
        CPostProcessingEffectChain(irr::video::CRenderer* renderer);
        /**
         * Destructor.
         */
        ~CPostProcessingEffectChain();

        /**
         * Attaches a new effect to this chain.
         * @param effect pointer to an effect to attach
         * @return Internal index in the chain of the newly attached effect.
         */
        irr::u32 attachEffect(irr::video::CPostProcessingEffect* effect);
        /**
         * Creates a new effect and attaches it to this chain.
         * @param effectShader shader to use for the new effect
         * @param callback callback for the shader, 0 for none
         * @return Pointer to the newly created effect.
         */
        irr::video::CPostProcessingEffect* createEffect(irr::video::SShaderSource &effectShader, irr::video::IShaderConstantSetCallBack* callback= new irr::video::IShaderDefaultPostProcessCallback);
        /**
         * Creates a new effect and attaches it to this chain.
         * @param type one of the included post-processing effect
         * @return The newly created effect.
         */
        irr::video::CPostProcessingEffect* createEffect(irr::video::E_POSTPROCESSING_EFFECT type);

        /**
         * Detaches the effect on the given index from the chain.
         * @param index the index of the effect in the chain
         */
        void detachEffect(irr::u32 index);
        /**
         * Detaches the effect pointed to by the pointer.
         * @param effect pointer to the effect whoch should be detached
         */
        void detachEffect(irr::video::CPostProcessingEffect* effect);

        /**
         * Removes the effect on the given index.
         * @param index the index of the effect in the chain
         */
        void removeEffect(irr::u32 index);
        /**
         * Removes the effect pointed to by the pointer.
         * @param effect pointer to the effect which should be detached
         */
        void removeEffect(irr::video::CPostProcessingEffect* effect);
        /**
         * Sets whether this chain should be active.
         * @param active whether the chain should be active or not
         */
        void setActive(bool active);
        /**
         * Returns whether is this chain active
         * @return True if active
         */
        bool isActive();

        void setKeepOriginalRender(bool k);
        bool getKeepOriginalRender() const;
        irr::video::ITexture* getOriginalRender();

        /**
         * Gets how many effects there are in this chain
         * @return The amount of effects belonging to this chain
         */
        irr::u32 getEffectCount() const;
        /**
         * Gets how many active effects there are in this chain
         * @return The amount of active effects belonging to this chain
         */
        irr::u32 getActiveEffectCount() const;
        /**
         * Gets pointer to the effect on the given index in the chain.
         * @param index index of the effect, starting from 0
         * @return Pointer to the effect on the given index.
         */
        irr::video::CPostProcessingEffect* getEffectFromIndex(irr::u32 index);
        /**
         * Returns the internal index of an effect pointed to by the pointer attached to this chain.
         * @param effect effect to return index for
         * @return The index of the effect.
         */
        irr::u32 getEffectIndex(irr::video::CPostProcessingEffect* effect) const;

    private:
        irr::video::CRenderer* Renderer;
        bool KeepOriginalRender;
        irr::video::ITexture* OriginalRender;
        bool Active;
        irr::core::array<irr::video::CPostProcessingEffect*> Effects;
};

}
}

#endif // CPOSTPROCESSINGEFFECTCHAIN_H
