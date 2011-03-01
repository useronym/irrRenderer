// This file is part of the "irrRenderer".
// For conditions of distribution and use, see copyright notice in irrRenderer.h

#ifndef ILIGHTMANAGERCUSTOM_H
#define ILIGHTMANAGERCUSTOM_H

#include <irrlicht.h>
#include "MaterialCallbacks.h"
#include <iostream>

namespace irr
{
namespace scene
{

class ILightManagerCustom : public irr::scene::ILightManager
{
    public:
        ILightManagerCustom(irr::IrrlichtDevice* device);

        ~ILightManagerCustom();

        virtual void OnPreRender(irr::core::array<irr::scene::ILightSceneNode*> &lightList);

        virtual void OnPostRender();

        virtual void OnRenderPassPreRender(irr::scene::E_SCENE_NODE_RENDER_PASS renderPass);

        virtual void OnRenderPassPostRender(irr::scene::E_SCENE_NODE_RENDER_PASS renderPass);

        virtual void OnNodePreRender(irr::scene::ISceneNode *node);

        virtual void OnNodePostRender(irr::scene::ISceneNode *node);

        void setMRTs(irr::core::array<irr::video::IRenderTarget> &mrts);

        void setFinalRenderTexture(irr::video::ITexture* tex);

        irr::video::ITexture* getFinalRenderTexture();

        void setFinalRenderToTexture(bool rtt);

        void setLightPointMaterialType(irr::video::E_MATERIAL_TYPE &type);

        void setLightPointCallback(irr::video::IShaderLightCallback* callback);


    private:
        irr::IrrlichtDevice* Device;
        irr::core::array<irr::video::IRenderTarget> MRTs;
        irr::video::ITexture* FinalRender;
        bool RenderToTexture;

        irr::video::E_MATERIAL_TYPE LightPointMaterial;
        irr::video::IShaderLightCallback* LightPointCallback;
};

}
}

#endif // ILIGHTMANAGERCUSTOM_H
