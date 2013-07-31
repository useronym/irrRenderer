#include "IShaderDefaultPostProcessCallback.h"

void irr::video::IShaderDefaultPostProcessCallback::OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData)
{
    irr::s32 tex0= 0;
    irr::s32 tex1= 1;
    irr::s32 tex2= 2;
    irr::s32 tex3= 3;
    services->setPixelShaderConstant("Render", &tex0, 1);
    services->setPixelShaderConstant("Tex0", &tex1, 1);
    services->setPixelShaderConstant("Tex1", &tex2, 1);
    services->setPixelShaderConstant("Tex2", &tex3, 1);

    //some more useful stuff for post processing shaders
    irr::core::dimension2du screensize= services->getVideoDriver()->getCurrentRenderTargetSize();
    irr::f32 pixelSizeX= 1.0/screensize.Width;
    irr::f32 pixelSizeY= 1.0/screensize.Height;
    services->setPixelShaderConstant("PixelSizeX", (irr::f32*)&pixelSizeX, 1);
    services->setPixelShaderConstant("PixelSizeY", (irr::f32*)&pixelSizeY, 1);
}
