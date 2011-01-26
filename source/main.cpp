#include "CRenderer.h"

CRenderer* createIrrRenderer(IrrlichtDevice* device, char* shaderDir= "shaders/")
{
    return new CRenderer(device, shaderDir);
}
