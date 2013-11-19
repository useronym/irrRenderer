// This file is part of the "irrRenderer".
// For conditions of distribution and use, see copyright notice in irrRenderer.h

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

    return getEffectIndex(effect);
}

irr::video::CPostProcessingEffect* irr::video::CPostProcessingEffectChain::createEffect(irr::video::SShaderSource &effectShader, irr::video::IShaderConstantSetCallBack* callback)
{
    irr::video::CPostProcessingEffect* newEff= Renderer->createPostProcessingEffect(effectShader, callback);
    attachEffect(newEff);

    return newEff;
}

irr::video::CPostProcessingEffect* irr::video::CPostProcessingEffectChain::createEffect(irr::video::E_POSTPROCESSING_EFFECT type)
{
    irr::video::CPostProcessingEffect* newEff= Renderer->createPostProcessingEffect(type);
    attachEffect(newEff);

    return newEff;
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


irr::u32 irr::video::CPostProcessingEffectChain::getEffectCount() const
{
    return Effects.size();
}

irr::u32 irr::video::CPostProcessingEffectChain::getActiveEffectCount() const
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


irr::u32 irr::video::CPostProcessingEffectChain::getEffectIndex(irr::video::CPostProcessingEffect* effect) const
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
