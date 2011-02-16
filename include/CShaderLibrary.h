#ifndef CSHADERS_H
#define CSHADERS_H

#include <irrlicht.h>
#include <fstream>
#include <string.h>
#include <iostream>

struct SShader
{
    irr::core::stringc Name;
    irr::core::stringc SourceVertex;
    irr::core::stringc SourcePixel;
};

class CShaderLibrary
{
    public:
        CShaderLibrary(irr::c8* shaderDir);
        void loadShader(irr::c8* name, irr::c8* sourceVertex, irr::c8* sourcePixel);
        SShader& getShader(irr::c8* name);

    private:
        irr::c8* ShaderDir;
        irr::core::array<SShader> Shaders;
};

#endif
