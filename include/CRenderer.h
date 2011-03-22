// This file is part of the "irrRenderer".
// For conditions of distribution and use, see copyright notice in irrRenderer.h

#ifndef CRENDERER_H
#define CRENDERER_H

#include <irrlicht.h>
#include "ILightManagerCustom.h"
#include "SMaterials.h"
#include "CShaderLibrary.h"
#include "MaterialCallbacks.h"
#include "IShaderDefaultPostProcessCallback.h"

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

        void addPostProcessingEffect(irr::video::SShaderSource &effectShader, irr::video::IShaderConstantSetCallBack* callback= new irr::video::IShaderDefaultPostProcessCallback);

        void enablePostProcessing(bool enable, irr::video::ECOLOR_FORMAT format= irr::video::ECF_A8R8G8B8);

        //! Removes all multiple render target textures - in case you want to create your own
        void clearMRTs();

        //! Adds your own multiple render target texture
        /*! \param name unique name of the texture, so please make sure it's unique
        \param dimension resolution of the texture, leave blank for auto (full screen resolution)
        */
        void addMRT(irr::c8* name, irr::video::ECOLOR_FORMAT format= irr::video::ECF_A8R8G8B8, irr::core::dimension2du dimension= irr::core::dimension2du(0,0));

        //! \return The RTT based on index \param index index of the MRT
        irr::video::ITexture* getMRT(irr::u32 index);

        //! \return The count of the MRT's
        irr::u32 getMRTCount();

        //! Should the final render be rendered to texture?
        void setDoFinalRenderToTexture(bool shouldI);

        //! \return if irr:video::CRenderer::setDoFinalRenderToTexture() was set to true, this will return the final render, otherwise 0
        irr::video::ITexture* getFinalRenderTexture();

        //! Add a new material
        /*! \param shader previously loaded irr::video::SShaderSource holding the source code, use irr::video::CShaderLibrary to load the shader
        \param callback custom callback if needed
        */
        irr::s32 addMaterial(irr::video::SShaderSource shader, irr::video::IShaderConstantSetCallBack *callback=0, irr::video::E_MATERIAL_TYPE baseType= irr::video::EMT_SOLID);

        //! Swaps the materials
        void swapMaterials();

        //! \return The irr::video::CShaderLibrary
        irr::video::CShaderLibrary* getShaderLibrary();

        //! \return irr::video::SMaterials structure which holds the irrlicht material enums
        irr::video::SMaterials* getMaterials();


    private:
        irr::IrrlichtDevice* Device;
        irr::scene::ILightManagerCustom* LightMgr;
        irr::video::CShaderLibrary* ShaderLib;
        irr::video::SMaterials* Materials;

        irr::core::array<irr::video::IRenderTarget> MRTs;
};

}
}

#endif // CRENDERER_H
