// This file is part of the "irrRenderer".
// For conditions of distribution and use, see copyright notice in irrRenderer.h

#ifndef CSHADERS_H
#define CSHADERS_H

#include <irrlicht.h>

namespace irr
{
namespace video
{

//! A structure holding shader data: it's name and source
struct SShaderSource
{
    irr::core::stringc Name;
    irr::core::stringc SourceVertex;
    irr::core::stringc SourcePixel;
};

//! A class for managing the shaders
/*! If you need to add your custom shader, this is the place to start. What this class does, is it loads the source codes
of your shaders under a name, so you can easily access them later. \n
Note: If you need to use this class, don't create a new one on your own, but use irr::video::CRenderer::getShaderLibrary() instead
*/
class CShaderLibrary
{
    public:
        //! Constructor, only used internally
        CShaderLibrary(const irr::c8* shaderDir, irr::IrrlichtDevice* dev);

        //! Loads a new GLSL shader from files
        /*! \param name a unique name of this shader
        \param sourceVertex name of the file holding the vertex source code, relative to the shader dir
        \param sourcePixel name of the file holding the pixel source code, relative to the shader dir
        */
        void loadShader(const irr::c8* name, const irr::c8* sourceVertex, const irr::c8* sourcePixel);

        //! Gets a previously loaded shader by name
        /*! \param name the unique name of the desired shader
        \return The shader, an irr::video::SShaderSource struct
        */
        SShaderSource& getShader(const irr::c8* name);

    private:
        irr::core::stringc ShaderDir;
        irr::IrrlichtDevice* Device;
        irr::core::array<SShaderSource> Shaders;
};

}
}

#endif
