// This file is part of the "irrRenderer".
// For conditions of distribution and use, see copyright notice in irrRenderer.h

#ifndef ILIGHTMANAGERCUSTOM_H
#define ILIGHTMANAGERCUSTOM_H

#include <irrlicht.h>

namespace irr
{
namespace video
{

class ILightManagerCustom : public irr::scene::ILightManager
{
    public:
        ILightManagerCustom(irr::scene::ISceneManager* smgr);

        ~ILightManagerCustom();

        void OnPreRender(irr::core::array<irr::scene::ILightSceneNode*> &lightList);

        void OnPostRender();

        void OnRenderPassPreRender(irr::scene::E_SCENE_NODE_RENDER_PASS renderPass);

        void OnRenderPassPostRender(irr::scene::E_SCENE_NODE_RENDER_PASS renderPass);

        void OnNodePreRender(irr::scene::ISceneNode *node);

        void OnNodePostRender(irr::scene::ISceneNode *node);


    private:
        irr::scene::ISceneManager* Smgr;
        irr::core::array<irr::scene::IMeshSceneNode*> LightMeshNodes;
};

}
}

#endif // ILIGHTMANAGERCUSTOM_H
