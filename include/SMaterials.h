// This file is part of the "irrRenderer".
// For conditions of distribution and use, see copyright notice in irrRenderer.h

#ifndef SMATERIALS_H_INCLUDED
#define SMATERIALS_H_INCLUDED

#include <irrlicht.h>

namespace irr
{
namespace video
{

//! Struct holding all predefined materials
struct SMaterials
{
    irr::video::E_MATERIAL_TYPE DeferredCompose;
    irr::video::E_MATERIAL_TYPE DeferredNormal;
    irr::video::E_MATERIAL_TYPE LightPoint;
    irr::video::E_MATERIAL_TYPE LightSpot;
    irr::video::E_MATERIAL_TYPE LightDirectional;
    irr::video::E_MATERIAL_TYPE LightAmbient;

    irr::video::E_MATERIAL_TYPE Solid;
    irr::video::E_MATERIAL_TYPE Water;
    irr::video::E_MATERIAL_TYPE DetailMap;
};

}
}

#endif
