#include "IShaderDefaultCallback.h"

void irr::video::IShaderDefaultCallback::OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData)
{
    int tex0= 0;
    int tex1= 1;
    services->setPixelShaderConstant("Tex0", (irr::f32*)&tex0, 1);
    services->setPixelShaderConstant("Tex1", (irr::f32*)&tex1, 1);

    irr::core::matrix4 viewMat = services->getVideoDriver()->getTransform(irr::video::ETS_VIEW);
    services->setVertexShaderConstant("VertexViewMat", viewMat.pointer(), 16);

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
