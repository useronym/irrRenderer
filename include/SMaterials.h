#ifndef SMATERIALS_H_INCLUDED
#define SMATERIALS_H_INCLUDED

#include <irrlicht.h>
using namespace irr;

struct SMaterials
{
    video::E_MATERIAL_TYPE DeferredCompose;
    video::E_MATERIAL_TYPE DeferredNormal;

    video::E_MATERIAL_TYPE Solid;
    video::E_MATERIAL_TYPE Water;
    video::E_MATERIAL_TYPE Terrain;
};

#endif
