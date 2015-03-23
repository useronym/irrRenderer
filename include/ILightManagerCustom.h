// This file is part of the "irrRenderer".
// For conditions of distribution and use, see copyright notice in irrRenderer.h

#ifndef ILIGHTMANAGERCUSTOM_H
#define ILIGHTMANAGERCUSTOM_H

#include <irrlicht.h>
#include "IQuadSceneNode.h"
#include "MaterialCallbacks.h"
#include "SMaterials.h"

namespace irr
{
namespace scene
{

class ILightManagerCustom : public irr::scene::ILightManager
{
    public:
        ILightManagerCustom(irr::IrrlichtDevice* device, irr::video::SMaterials* mats);

        ~ILightManagerCustom();

        virtual void OnPreRender(core::array<ISceneNode*> & lightList);

        virtual void OnPostRender();

        virtual void OnRenderPassPreRender(irr::scene::E_SCENE_NODE_RENDER_PASS renderPass);

        virtual void OnRenderPassPostRender(irr::scene::E_SCENE_NODE_RENDER_PASS renderPass);

        virtual void OnNodePreRender(irr::scene::ISceneNode *node);

        virtual void OnNodePostRender(irr::scene::ISceneNode *node);


        void setRenderTarget(video::IRenderTarget *RenderTarget);

        void setDoFinalRenderIntoTexture(bool well);

        bool getDoFinalRenderToTexture() const;

        irr::video::ITexture* getRenderTexture();


        void setLightPointMaterialType(irr::video::E_MATERIAL_TYPE &type);
        void setLightPointCallback(irr::video::PointCallback* callback);
        void setLightSpotMaterialType(irr::video::E_MATERIAL_TYPE &type);
        void setLightSpotCallback(irr::video::SpotCallback* callback);
        void setLightDirectionalMaterialType(irr::video::E_MATERIAL_TYPE &type);
        void setLightDirectionalCallback(irr::video::DirectionalCallback* callback);
        void setLightAmbientMaterialType(irr::video::E_MATERIAL_TYPE &type);
        void setLightAmbientCallback(irr::video::AmbientCallback* callback);


    private:
        irr::IrrlichtDevice* Device;
        irr::video::SMaterials* Materials;
        irr::video::IRenderTarget *RenderTarget;

        // renders the lighting
        inline void deferred();

        //set to true if we're currently rendering transparent nodes
        bool TransparentRenderPass;

        bool FinalRenderToTexture;
        irr::video::ITexture* FinalRender;


        irr::scene::IMeshSceneNode* LightSphere, *LightCone;
        irr::scene::IQuadSceneNode* LightQuad;


        irr::video::E_MATERIAL_TYPE LightPointMaterial;
        irr::video::PointCallback* LightPointCallback;

        irr::video::E_MATERIAL_TYPE LightSpotMaterial;
        irr::video::SpotCallback* LightSpotCallback;

        irr::video::E_MATERIAL_TYPE LightDirectionalMaterial;
        irr::video::DirectionalCallback* LightDirectionalCallback;

        irr::video::E_MATERIAL_TYPE LightAmbientMaterial;
        irr::video::AmbientCallback* LightAmbientCallback;
};

}
}

#endif // ILIGHTMANAGERCUSTOM_H
