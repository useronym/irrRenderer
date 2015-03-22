// This file is part of the "irrRenderer".
// For conditions of distribution and use, see copyright notice in irrRenderer.h

#include "IShaderDefaultCallback.h"

void irr::video::IShaderDefaultCallback::OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData)
{
    irr::s32 tex0= 0;
    irr::s32 tex1= 1;
    services->setPixelShaderConstant("Tex0", &tex0, 1);
    services->setPixelShaderConstant("Tex1", &tex1, 1);

    irr::core::matrix4 worldMat = services->getVideoDriver()->getTransform(irr::video::ETS_WORLD);
    irr::core::matrix4 viewMat = services->getVideoDriver()->getTransform(irr::video::ETS_VIEW);
    irr::core::matrix4 projMat = services->getVideoDriver()->getTransform(irr::video::ETS_PROJECTION);
    irr::core::matrix4 worldViewProj = projMat * viewMat * worldMat;//worldMat * viewMat * projMat;
    irr::core::matrix4 worldView = viewMat * worldMat;

    services->setVertexShaderConstant("WorldViewProjMat", worldViewProj.pointer(), 16);
    services->setVertexShaderConstant("WorldViewMat", worldView.pointer(), 16);

    irr::core::matrix4 mat = services->getVideoDriver()->getTransform(irr::video::ETS_PROJECTION);
    irr::f32 farDist;
    if (mat[10]>0.f) farDist = -mat[14]/(mat[10]-1.f); // Left Handed
    else farDist = mat[14]/(mat[10]+1.f); // Right Handed
    services->setVertexShaderConstant("CamFar", &farDist, 1);

    services->setPixelShaderConstant("Lighting", &Lighting, 1);

    services->setPixelShaderConstant("Param1", &Param1, 1);
    services->setPixelShaderConstant("Param2", &Param2, 1);
}

void irr::video::IShaderDefaultCallback::OnSetMaterial (const irr::video::SMaterial &material)
{
    Lighting= irr::f32(material.Lighting);

    Param1= material.MaterialTypeParam;
    Param2= material.MaterialTypeParam2;
}
