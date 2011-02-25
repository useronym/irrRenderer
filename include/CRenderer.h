// This file is part of the "irrRenderer".
// For conditions of distribution and use, see copyright notice in irrRenderer.h

#ifndef CRENDERER_H
#define CRENDERER_H

#include <irrlicht.h>
#include "SMaterials.h"
#include "CShaderLibrary.h"
#include "MaterialCallbacks.h"

namespace irr
{
namespace video
{

//! The base class holding all other stuff
class CRenderer
{
    public:
        //! Constructor, only used internally
        CRenderer(irr::IrrlichtDevice* device, irr::c8* shaderDir);

        ~CRenderer();

        //! Loads the default pipeline \n Note: If you haven't altered the pipeline, it doesn't make sense to call this
        void createDefaultPipeline();

        //! Removes all multiple render target textures - in case you want to create your own
        void clearMRTs();

        //! Adds your own multiple render target
        /*! \param name unique name of the texture, so please make sure it's unique
        \param dimension resolution of the texture, leave blank for auto
        */
        void addMRT(irr::c8* name, irr::core::dimension2du dimension= irr::core::dimension2du(0,0));

        //! Get the Materials structure which holds the irrlicht material enums
        irr::video::SMaterials* getMaterials();

        //! Add a new material
        /*! \param shader previously loaded SShader holding the source code, use CShaderLibrary to load the shader
        \param callback custom callback if needed
        */
        irr::s32 addMaterial(irr::video::SShader shader, irr::video::IShaderConstantSetCallBack *callback=0);

        //! Draws the scene, use instead of ISceneManager::drawAll()
        void drawAll();

        //! \return The irr::video::CShaderLibrary
        irr::video::CShaderLibrary* getShaderLibrary();

        //! \return The color buffer
        irr::video::ITexture* getColorBuffer();

        //! \return The normal+depth buffer
        irr::video::ITexture* getNormalBuffer();


    private:
        irr::IrrlichtDevice* Device;
        irr::video::CShaderLibrary* ShaderLib;
        irr::video::SMaterials* Materials;

        irr::core::array<irr::video::IRenderTarget> MRTs;

        irr::scene::ISceneNode* ScreenQuad;
};

}
}

#endif // CRENDERER_H
