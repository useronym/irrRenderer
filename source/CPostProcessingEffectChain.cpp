#include "CPostProcessingEffectChain.h"

irr::video::CPostProcessingEffectChain::CPostProcessingEffectChain(irr::video::CRenderer* renderer)
{
    Renderer= renderer;
}

irr::video::CPostProcessingEffectChain::~CPostProcessingEffectChain()
{
    //dtor
}

irr::u32 irr::video::CPostProcessingEffectChain::addEffect(irr::video::CPostProcessingEffect* effect)
{
    Effects.push_back(effect);
}

irr::video::CPostProcessingEffect* irr::video::CPostProcessingEffectChain::createEffect(irr::video::SShaderSource &effectShader, irr::video::IShaderConstantSetCallBack* callback)
{
    addEffect(Renderer->createPostProcessingEffect(effectShader, callback));
}

irr::video::CPostProcessingEffect* irr::video::CPostProcessingEffectChain::createEffect(irr::video::E_POSTPROCESSING_EFFECT type)
{

}


bool irr::video::CPostProcessingEffectChain::removeEffect(irr::u32 index)
{

}

bool irr::video::CPostProcessingEffectChain::removeEffect(irr::video::CPostProcessingEffect* effect)
{

}


irr::video::CPostProcessingEffect* irr::video::CPostProcessingEffectChain::getEffect(irr::u32 index)
{

}


void irr::video::CPostProcessingEffectChain::setActive(bool active)
{

}

bool irr::video::CPostProcessingEffectChain::isActive()
{

}
