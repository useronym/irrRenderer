#include "CPostProcessingEffect.h"

irr::video::CPostProcessingEffect::CPostProcessingEffect(irr::video::E_MATERIAL_TYPE &type, irr::video::IShaderConstantSetCallBack* callback)
{
    Active= true;
    MaterialType= type;
    Callback= callback;
}

irr::video::CPostProcessingEffect::~CPostProcessingEffect()
{
    //dtor
}

void irr::video::CPostProcessingEffect::addTextureToShader(irr::video::ITexture* tex)
{
    TexturesToPass.push_back(tex);
}

irr::video::ITexture* irr::video::CPostProcessingEffect::getTextureToPass(irr::u32 index)
{
    return TexturesToPass[index];
}

irr::u32 irr::video::CPostProcessingEffect::getTextureToPassCount()
{
    return TexturesToPass.size();
}

irr::video::E_MATERIAL_TYPE& irr::video::CPostProcessingEffect::getMaterialType()
{
    return MaterialType;
}

irr::video::IShaderConstantSetCallBack* irr::video::CPostProcessingEffect::getCallback()
{
    return Callback;
}

void irr::video::CPostProcessingEffect::setActive(bool active)
{
    Active= active;
}

bool irr::video::CPostProcessingEffect::isActive()
{
    return Active;
}
