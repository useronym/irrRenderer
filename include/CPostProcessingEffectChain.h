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
        CPostProcessingEffectChain(irr::video::CRenderer* renderer);
        ~CPostProcessingEffectChain();

        irr::u32 attachEffect(irr::video::CPostProcessingEffect* effect);
        irr::video::CPostProcessingEffect* createEffect(irr::video::SShaderSource &effectShader, irr::video::IShaderConstantSetCallBack* callback= new irr::video::IShaderDefaultPostProcessCallback);
        irr::video::CPostProcessingEffect* createEffect(irr::video::E_POSTPROCESSING_EFFECT type);

        void detachEffect(irr::u32 index);
        void detachEffect(irr::video::CPostProcessingEffect* effect);

        irr::u32 getEffectCount();
        irr::video::CPostProcessingEffect* getEffect(irr::u32 index);
        irr::u32 getEffectIndex(irr::video::CPostProcessingEffect* effect);

        void setActive(bool active);
        bool isActive();

    private:
        irr::video::CRenderer* Renderer;
        bool Active;
        irr::core::array<irr::video::CPostProcessingEffect*> Effects;
};

}
}

#endif // CPOSTPROCESSINGEFFECTCHAIN_H
