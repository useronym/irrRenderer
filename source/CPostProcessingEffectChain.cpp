#include "CPostProcessingEffectChain.h"
#include "CRenderer.h"

irr::video::CPostProcessingEffectChain::CPostProcessingEffectChain(irr::video::CRenderer* renderer)
{
    Renderer= renderer;
    Active= true;
}

irr::video::CPostProcessingEffectChain::~CPostProcessingEffectChain()
{
    //dtor
}

irr::u32 irr::video::CPostProcessingEffectChain::attachEffect(irr::video::CPostProcessingEffect* effect)
{
    if(effect->getChain())effect->getChain()->detachEffect(effect);
    effect->setChain(this);
    Effects.push_back(effect);
}

irr::video::CPostProcessingEffect* irr::video::CPostProcessingEffectChain::createEffect(irr::video::SShaderSource &effectShader, irr::video::IShaderConstantSetCallBack* callback)
{
    attachEffect(Renderer->createPostProcessingEffect(effectShader, callback));
}

irr::video::CPostProcessingEffect* irr::video::CPostProcessingEffectChain::createEffect(irr::video::E_POSTPROCESSING_EFFECT type)
{
    attachEffect(Renderer->createPostProcessingEffect(type));
}


void irr::video::CPostProcessingEffectChain::detachEffect(irr::u32 index)
{
    Effects[index]->setChain(0);
    Effects.erase(index);
}

void irr::video::CPostProcessingEffectChain::detachEffect(irr::video::CPostProcessingEffect* effect)
{
    detachEffect(getEffectIndex(effect));
}


void irr::video::CPostProcessingEffectChain::removeEffect(irr::u32 index)
{
    delete Effects[index];
    Effects.erase(index);
}


irr::u32 irr::video::CPostProcessingEffectChain::getEffectCount()
{
    return Effects.size();
}

irr::u32 irr::video::CPostProcessingEffectChain::getActiveEffectCount()
{
    irr::u32 count= 0;
    for(irr::u32 i= 0; i < Effects.size(); i++)
    {
        if(Effects[i]->isActive()) count++;
    }
    return count;
}


irr::video::CPostProcessingEffect* irr::video::CPostProcessingEffectChain::getEffectFromIndex(irr::u32 index)
{
    return Effects[index];
}


irr::u32 irr::video::CPostProcessingEffectChain::getEffectIndex(irr::video::CPostProcessingEffect* effect)
{
    for(irr::u32 i= 0; i < Effects.size(); i++)
    {
        if(Effects[i] == effect) return i;
    }
    return 0;
}


void irr::video::CPostProcessingEffectChain::setActive(bool active)
{
    Active= active;
}

bool irr::video::CPostProcessingEffectChain::isActive()
{
    return Active;
}
