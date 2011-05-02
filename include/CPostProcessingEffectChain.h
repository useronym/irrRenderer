#ifndef CPOSTPROCESSINGEFFECTCHAIN_H
#define CPOSTPROCESSINGEFFECTCHAIN_H

#include <irrlicht.h>
#include "CRenderer.h"
#include "CPostProcessingEffect.h"

namespace irr
{
namespace video
{

class CPostProcessingEffectChain
{
    public:
        CPostProcessingEffectChain(irr::video::CRenderer* renderer);
        ~CPostProcessingEffectChain();

        irr::u32 attachEffect(irr::video::CPostProcessingEffect* effect);
        irr::video::CPostProcessingEffect* createEffect(irr::video::SShaderSource &effectShader, irr::video::IShaderConstantSetCallBack* callback= new irr::video::IShaderDefaultPostProcessCallback);
        irr::video::CPostProcessingEffect* createEffect(irr::video::E_POSTPROCESSING_EFFECT type);

        bool detachEffect(irr::u32 index);
        bool detachEffect(irr::video::CPostProcessingEffect* effect);

        irr::video::CPostProcessingEffect* getEffect(irr::u32 index);

        void setActive(bool active);
        bool isActive();

    private:
        irr::video::CRenderer* Renderer;
        irr::core::array<irr::video::CPostProcessingEffect*> Effects;
};

}
}

#endif // CPOSTPROCESSINGEFFECTCHAIN_H
