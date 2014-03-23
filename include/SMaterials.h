// This file is part of the "irrRenderer".
// For conditions of distribution and use, see copyright notice in irrRenderer.h

#ifndef SMATERIALS_H_INCLUDED
#define SMATERIALS_H_INCLUDED

#include <irrlicht.h>

namespace irr
{
namespace video
{

/**
 * Structure holding all materials irrRenderer uses.
 */
struct SMaterials
{
    /** \brief A standard solid material
     */
    irr::video::E_MATERIAL_TYPE Solid;
    /** \brief Transparent alpha reference material
     *
     * Each texel is either fully transparent or solid. \n
     * Works just as Irrlicht's irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF.
     */
    irr::video::E_MATERIAL_TYPE TransparentRef;
    /** \brief Normal map
     *
     * Requires a normal map texture and a mesh with tangents.
     */
    irr::video::E_MATERIAL_TYPE Normal;
    /** \brief Normal map for animated meshes
     *
     * Requires a normal map texture, however does NOT require a tangent mesh, which means it can be easily used on Irrlicht's animated meshes. \n
     * Approximates the tangent in code, results in lower quality.
     */
    irr::video::E_MATERIAL_TYPE NormalAnimated;
    /** \brief Parallax map
     *
     * Requires a normal map texture with the original height stored in the alpha channel. Plus a tangent mesh. \n
     * Param1 affects bumpiness.
     */
    irr::video::E_MATERIAL_TYPE Parallax;
    /** \brief Detail map
     *
     * Meant as a replacement for Irrlicht's irr::video::EMT_DETAIL_MAP. \n
     * First texture is applied as a solid material. Then, it's mixed with the second texture, which is the detail map. \n
     * Param1 affects detail map repeat.
     **/
    irr::video::E_MATERIAL_TYPE DetailMap;

    /** Only used internally. **/
    irr::video::E_MATERIAL_TYPE LightPoint;
    /** Only used internally. **/
    irr::video::E_MATERIAL_TYPE LightSpot;
    /** Only used internally. **/
    irr::video::E_MATERIAL_TYPE LightDirectional;
    /** Only used internally. **/
    irr::video::E_MATERIAL_TYPE LightAmbient;
};

}
}

#endif
