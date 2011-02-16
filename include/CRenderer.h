//FAQ
//
//Q: Is this a kind of magic?
//A: It's A Kind Of Magic!
//

#ifndef CRENDERER_H
#define CRENDERER_H

#include <irrlicht.h>
#include "SMaterials.h"
#include "CShaderLibrary.h"
#include "MaterialCallbacks.h"
using namespace irr;

class CRenderer
{
    public:
        CRenderer(IrrlichtDevice* device, char* shaderDir);

        ~CRenderer();

        SMaterials* getMaterials();

        irr::s32 addMaterial(SShader shader, irr::video::IShaderConstantSetCallBack *callback=0);

        void drawAll();

        video::ITexture* getColorBuffer();

        video::ITexture* getNormalBuffer();


    private:
        IrrlichtDevice* Device;
        CShaderLibrary* ShaderLib;
        SMaterials* Materials;

        core::array<video::IRenderTarget> MRTs;

        scene::ISceneNode* ScreenQuad;
};

#endif // CRENDERER_H
