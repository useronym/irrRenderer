#include <irrlicht.h>
#include <irrRenderer.h>

namespace irr
{
    CRenderer* createRenderer(irr::IrrlichtDevice* device, char* shaderDir)
    {
        return new CRenderer(device, shaderDir);
    }
}
