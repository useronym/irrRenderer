#ifndef IRRRENDERER_H_INCLUDED
#define IRRRENDERER_H_INCLUDED

#include "CRenderer.h"
#include "CShaderLibrary.h"
#include "MaterialCallbacks.h"
#include "SMaterials.h"

namespace irr
{
    CRenderer* createRenderer(IrrlichtDevice* device, char* shaderDir= "shaders/");
}

#endif // IRRRENDERER_H_INCLUDED
