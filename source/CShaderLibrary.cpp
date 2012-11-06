// This file is part of the "irrRenderer".
// For conditions of distribution and use, see copyright notice in irrRenderer.h

#include "CShaderLibrary.h"

irr::video::CShaderLibrary::CShaderLibrary(irr::c8* shaderDir)
{
    ShaderDir= shaderDir;

    //create empty first shader if someone is stupid enough to request a non-existent shader
    Shaders.push_back(SShaderSource());
}

void irr::video::CShaderLibrary::loadShader(irr::c8* name, irr::c8* sourceVertex, irr::c8* sourcePixel)
{
    SShaderSource newShader;
    newShader.Name= name;

    irr::core::stringc fileNameVertex;
    fileNameVertex.append(ShaderDir);
    fileNameVertex.append(sourceVertex);
    std::ifstream fileVertex;
    fileVertex.open(fileNameVertex.c_str());
    if(fileVertex.is_open())
    {
        while(fileVertex.good())
        {
            irr::c8 buff[512];
            fileVertex.getline(buff, 512);
            newShader.SourceVertex.append(buff);
            newShader.SourceVertex.append("\n");
        }
        fileVertex.close();
    }

    irr::core::stringc fileNamePixel;
    fileNamePixel.append(ShaderDir);
    fileNamePixel.append(sourcePixel);
    std::ifstream filePixel;
    filePixel.open(fileNamePixel.c_str());
    if(filePixel.is_open())
    {
        while(filePixel.good())
        {
            irr::c8 buff[512];
            filePixel.getline(buff, 512);
            newShader.SourcePixel.append(buff);
            newShader.SourcePixel.append("\n");
        }
        filePixel.close();
    }

    Shaders.push_back(newShader);
}

irr::video::SShaderSource& irr::video::CShaderLibrary::getShader(irr::c8* name)
{
    for(irr::u16 i= 0; i < Shaders.size(); i++)
    {
        if(Shaders[i].Name == name) return Shaders[i];
    }
    return Shaders[0];
}
