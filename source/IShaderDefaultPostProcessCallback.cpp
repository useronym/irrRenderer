#include "IShaderDefaultPostProcessCallback.h"

void irr::video::IShaderDefaultPostProcessCallback::OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData)
{
    int tex0= 0;
    int tex1= 1;
    int tex2= 2;
    int tex3= 3;
    services->setPixelShaderConstant("Render", (float*)&tex0, 1);
    services->setPixelShaderConstant("MRT1", (float*)&tex1, 1);
    services->setPixelShaderConstant("MRT2", (float*)&tex2, 1);
    services->setPixelShaderConstant("MRT3", (float*)&tex3, 1);
}
