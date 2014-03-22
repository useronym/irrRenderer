// This file is part of the "irrRenderer".
// For conditions of distribution and use, see copyright notice in irrRenderer.h

#include "CPostProcessingEffectChain.h"
#include "CRenderer.h"

irr::video::CPostProcessingEffectChain::CPostProcessingEffectChain(irr::video::CRenderer* renderer)
    :Renderer(renderer),
    Video(Renderer->getDevice()->getVideoDriver()),
    ActiveEffectCount(0),
    OriginalRender(0),
    Active(false)
{

}

irr::video::CPostProcessingEffectChain::~CPostProcessingEffectChain()
{
    if(OriginalRender)
    {
        Video->removeTexture(OriginalRender);
        OriginalRender = 0;
    }
}

irr::u32 irr::video::CPostProcessingEffectChain::attachEffect(irr::video::CPostProcessingEffect* effect)
{
    if(effect->getChain())effect->getChain()->detachEffect(effect);
    effect->setChain(this);
    Effects.push_back(effect);
    Active = true;
    ActiveEffectCount++;

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

    ActiveEffectCount--;
    if(getActiveEffectCount() == 0)
    {
        Active = false;
    }
}

void irr::video::CPostProcessingEffectChain::detachEffect(irr::video::CPostProcessingEffect* effect)
{
    detachEffect(getEffectIndex(effect));
}


void irr::video::CPostProcessingEffectChain::removeEffect(irr::u32 index)
{
    removeEffect(getEffectFromIndex(index));
}

void irr::video::CPostProcessingEffectChain::removeEffect(irr::video::CPostProcessingEffect* effect)
{
    detachEffect(effect);
    delete effect;
}


void irr::video::CPostProcessingEffectChain::setKeepOriginalRender(bool k)
{
    if(k && !getKeepOriginalRender())
    {
        irr::core::dimension2du dimension= Video->getCurrentRenderTargetSize();
        OriginalRender = Video->addRenderTargetTexture(dimension, "Chain-original-texture" + Video->getTextureCount());
    }
    else if(!k && getKeepOriginalRender())
    {
        Video->removeTexture(OriginalRender);
        OriginalRender = 0;
    }
}

bool irr::video::CPostProcessingEffectChain::getKeepOriginalRender() const
{
    return (OriginalRender != 0);
}

irr::video::ITexture* irr::video::CPostProcessingEffectChain::getOriginalRender()
{
    return OriginalRender;
}

void irr::video::CPostProcessingEffectChain::requestActiveEffectUpdate()
{
    ActiveEffectCount = 0;
    for(irr::u32 i = 0; i < Effects.size(); i++)
    {
        if(Effects[i]->isActive()) ActiveEffectCount++;
    }
}

irr::u32 irr::video::CPostProcessingEffectChain::getEffectCount() const
{
    return Effects.size();
}

irr::u32 irr::video::CPostProcessingEffectChain::getActiveEffectCount() const
{
    return ActiveEffectCount;
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
    if(getActiveEffectCount() == 0)
    {
        setActive(false);
    }

    return Active;
}
