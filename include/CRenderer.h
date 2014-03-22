// This file is part of the "irrRenderer".
// For conditions of distribution and use, see copyright notice in irrRenderer.h

#ifndef CRENDERER_H
#define CRENDERER_H

#include <irrlicht.h>

#include "ILightManagerCustom.h"
#include "SMaterials.h"
#include "CShaderLibrary.h"
#include "MaterialCallbacks.h"
#include "CMaterialSwapper.h"
#include "IShaderDefaultCallback.h"


namespace irr
{
namespace video
{

//! The base class holding all other stuff.
/**
 * You can create one by calling irr::video::createRenderer(IrrlichtDevice).
 */
class CRenderer
{
    public:
        /**
         * Constructor, only used internally.
         * @param device Irrlicht device
         * @param shaderDir relative path to the shaders
         */
        CRenderer(irr::IrrlichtDevice* device, const irr::c8* shaderDir);

        /**
         * Destructor.
         */
        ~CRenderer();

        /** Loads the default pipeline
         * \n Note: If you haven't altered the pipeline, it doesn't make sense to call this
         */
        void createDefaultPipeline();


        /**
         * Removes all multiple render target textures - in case you want to create your own
         */
        void clearMRTs();

        /**
         * Adds your own multiple render target texture
         * @param name unique name of the texture, so please make sure it's unique
         * @param dimension resolution of the texture, leave blank for auto (full screen resolution)
         */
        void createMRT(const irr::c8* name, irr::video::ECOLOR_FORMAT format= irr::video::ECF_A8R8G8B8, irr::core::dimension2du dimension= irr::core::dimension2du(0,0));

        /**
         * @return The RTT based on index @param index index of the MRT
         */
        irr::video::ITexture* getMRT(irr::u32 index);

        /**
         * @return The count of the MRT's..
         */
        irr::u32 getMRTCount() const;

        /**
         * Should the final render be rendered to texture?
         * @param shouldI true if yes
         */
        void setDoFinalRenderToTexture(bool shouldI);

        /**
         * @return If irr:video::CRenderer::setDoFinalRenderToTexture() was set to true, this will return the final render, otherwise 0.
         */
        irr::video::ITexture* getFinalRenderTexture() const;

        /**
         * Add a new material
         * @param shader previously loaded irr::video::SShaderSource holding the source code, use irr::video::CShaderLibrary to load the shader
         * @param callback custom callback if needed
         */
        irr::s32 createMaterial(irr::video::SShaderSource shader, irr::video::IShaderConstantSetCallBack *callback=0, irr::video::E_MATERIAL_TYPE baseType= irr::video::EMT_SOLID);

        /**
         * @return Managed instance of the irr::video::CShaderLibrary.
         */
        irr::video::CShaderLibrary* getShaderLibrary() const;

        /**
         * @return irr::video::SMaterials structure which holds the shader materials used by irrRenderer.
         */
        irr::video::SMaterials* getMaterials() const;

        /**
         * Returns the material swapper. Use it to swap irrlicht materials to irrrenderer materials easily.
         * @return An instance of the material swapper.
         */
        irr::video::CMaterialSwapper* getMaterialSwapper() const;

        /**
         * @return Irrlicht device this renderer uses.
         */
        irr::IrrlichtDevice* getDevice() const;


    private:
        void loadShaders();

        irr::IrrlichtDevice* Device;
        irr::scene::ILightManagerCustom* LightMgr;
        irr::video::CShaderLibrary* ShaderLib;
        irr::video::SMaterials* Materials;
        irr::video::CMaterialSwapper* MaterialSwapper;

        irr::core::array<irr::video::IRenderTarget> MRTs;
};

}
}

#endif // CRENDERER_H
