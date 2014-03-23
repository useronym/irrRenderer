// This file is part of the "irrRenderer".
// For conditions of distribution and use, see copyright notice in irrRenderer.h

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

//! A class to help you with setting the correct materials on your nodes.
/**
 * Get an instance from irr::video::CRenderer and call swapMaterials() to automatically set the correct irrRenderer materials on your scene.
 * By default, swaps these materials to their irrRenderer alternatives: irr::video::EMT_SOLID, irr::video::EMT_DETAIL_MAP, irr::video::EMT_NORMAL_MAP_SOLID, irr::video::EMT_PARALLAX_MAP_SOLID, irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF.
 * Use updateEntry() and removeEntry() to alter these.
 */
class CMaterialSwapper
{
    public:
        /**
         * Constructor, only used internally.
         * @param smgr the scene manager this swapper should operate on
         * @param materials the materials struct to use
         */
        CMaterialSwapper(irr::scene::ISceneManager* smgr, irr::video::SMaterials* materials);

        /**
         * Destructor.
         */
        ~CMaterialSwapper();

        /**
         * Swaps materials on the scene node pointed to by the pointer. If 0, swaps materials on all the scene nodes in the current scene.
         * @param node pointer to a specific node, if any
         */
        void swapMaterials(irr::scene::ISceneNode* node = 0) const;

        /**
         * Updates entry for a given material. If the material swapper finds the given material type, it will change it to the suggested new material type. If there's no entry in the "database", keeps the original material.
         * @param swapFrom material which should be swapped to a different one
         * @param swapTo new material to which it should be swapped
         */
        void updateEntry(irr::video::E_MATERIAL_TYPE swapFrom, irr::video::E_MATERIAL_TYPE swapTo);

        /**
         * Removes the selected entry from the "database".
         * \param swapFrom entry which should be erased
         */
        void removeEntry(irr::video::E_MATERIAL_TYPE swapFrom);

    private:
        irr::scene::ISceneManager* Smgr;
        irr::video::SMaterials* Materials;
        irr::core::array<irr::video::SMaterialSwapperEntry*> Entries;

        void swapMaterialsOnNode(irr::scene::ISceneNode* node) const;

};

}
}

#endif // CMATERIALSWAPPER_H
