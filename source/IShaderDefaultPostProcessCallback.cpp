#include "IShaderDefaultPostProcessCallback.h"

void irr::video::IShaderDefaultPostProcessCallback::OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData)
{
    int tex0= 0;
    int tex1= 1;
    int tex2= 2;
    int tex3= 3;
    services->setPixelShaderConstant("Render", (float*)&tex0, 1);
    services->setPixelShaderConstant("MRT0", (float*)&tex1, 1);
    services->setPixelShaderConstant("MRT1", (float*)&tex2, 1);
    services->setPixelShaderConstant("MRT2", (float*)&tex3, 1);

    //some more useful stuff for post processing shaders
    irr::core::dimension2du screensize= services->getVideoDriver()->getCurrentRenderTargetSize();
    irr::f32 pixelSizeX= 1.0/screensize.Width;
    irr::f32 pixelSizeY= 1.0/screensize.Height;
    services->setPixelShaderConstant("PixelSizeX", (float*)&pixelSizeX, 1);
    services->setPixelShaderConstant("PixelSizeY", (float*)&pixelSizeY, 1);
}
