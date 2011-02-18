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

        //! Get the Materials structure which holds the irrlicht material enums
        SMaterials* getMaterials();

        //! Add a new material
        /*! \param shader previously loaded SShader holding the source code, use CShaderLibrary to load the shader
        \param callback custom callback if needed
        */
        irr::s32 addMaterial(SShader shader, irr::video::IShaderConstantSetCallBack *callback=0);

        //! Draws the scene, use instead of ISceneManager::drawAll()
        void drawAll();

        //! \return The irr::video::CShaderLibrary
        CShaderLibrary* getShaderLibrary();

        //! \return The color buffer
        irr::video::ITexture* getColorBuffer();

        //! \return The normal+depth buffer
        irr::video::ITexture* getNormalBuffer();


    private:
        irr::IrrlichtDevice* Device;
        CShaderLibrary* ShaderLib;
        SMaterials* Materials;

        irr::core::array<irr::video::IRenderTarget> MRTs;

        irr::scene::ISceneNode* ScreenQuad;
};

}
}

#endif // CRENDERER_H
