#ifndef CMATERIALSWAPPER_H
#define CMATERIALSWAPPER_H

#include <irrlicht.h>
#include "SMaterials.h"

namespace irr
{
namespace video
{

struct SMaterialSwapperEntry
{
    irr::video::E_MATERIAL_TYPE SwapFrom;
    irr::video::E_MATERIAL_TYPE SwapTo;
};

class CMaterialSwapper
{
    public:
        CMaterialSwapper(irr::scene::ISceneManager* smgr, irr::video::SMaterials* materials);

        ~CMaterialSwapper();

        void swapMaterials(irr::scene::ISceneNode* node= 0);

        void updateEntry(irr::video::E_MATERIAL_TYPE swapFrom, irr::video::E_MATERIAL_TYPE swapTo);

    private:
        irr::scene::ISceneManager* Smgr;
        irr::video::SMaterials* Materials;
        irr::core::array<irr::video::SMaterialSwapperEntry*> Entries;

        void swapMaterialsOnNode(irr::scene::ISceneNode* node);

};

}
}

#endif // CMATERIALSWAPPER_H
