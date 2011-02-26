// This file is part of the "irrRenderer".
// For conditions of distribution and use, see copyright notice in irrRenderer.h

#ifndef ILIGHTMANAGERCUSTOM_H
#define ILIGHTMANAGERCUSTOM_H

#include <irrlicht.h>

namespace irr
{
namespace scene
{

class ILightManagerCustom : public irr::scene::ILightManager
{
    public:
        ILightManagerCustom(irr::IrrlichtDevice* device);

        ~ILightManagerCustom();

        void OnPreRender(irr::core::array<irr::scene::ILightSceneNode*> &lightList);

        void OnPostRender();

        void OnRenderPassPreRender(irr::scene::E_SCENE_NODE_RENDER_PASS renderPass);

        void OnRenderPassPostRender(irr::scene::E_SCENE_NODE_RENDER_PASS renderPass);

        void OnNodePreRender(irr::scene::ISceneNode *node);

        void OnNodePostRender(irr::scene::ISceneNode *node);

        void setMRTs(irr::core::array<irr::video::IRenderTarget> &mrts);

        void setFinalRenderTexture(irr::video::ITexture* tex);

        irr::video::ITexture* getFinalRenderTexture();

        void setRenderToTexture(bool rtt);


    private:
        irr::IrrlichtDevice* Device;
        irr::core::array<irr::video::IRenderTarget> MRTs;
        irr::video::ITexture* FinalRender;
        bool RenderToTexture;
};

}
}

#endif // ILIGHTMANAGERCUSTOM_H
