#include "IShaderDefaultPostProcessCallback.h"

void irr::video::IShaderDefaultPostProcessCallback::OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData)
{
    int tex0= 0;
    int tex1= 1;
    int tex2= 2;
    int tex3= 3;
    services->setPixelShaderConstant("Render", (irr::f32*)&tex0, 1);
    services->setPixelShaderConstant("Tex0", (irr::f32*)&tex1, 1);
    services->setPixelShaderConstant("Tex1", (irr::f32*)&tex2, 1);
    services->setPixelShaderConstant("Tex2", (irr::f32*)&tex3, 1);

    //some more useful stuff for post processing shaders
    irr::core::dimension2du screensize= services->getVideoDriver()->getCurrentRenderTargetSize();
    irr::f32 pixelSizeX= 1.0/screensize.Width;
    irr::f32 pixelSizeY= 1.0/screensize.Height;
    services->setPixelShaderConstant("PixelSizeX", (irr::f32*)&pixelSizeX, 1);
    services->setPixelShaderConstant("PixelSizeY", (irr::f32*)&pixelSizeY, 1);
}
