
/* Copyright (C) 2010-2013 Adam 'entity' Krupicka

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Please note that the Irrlicht Engine is based in part on the work of the
  Independent JPEG Group, the zlib and the libPng. This means that if you use
  the Irrlicht Engine in your product, you must acknowledge somewhere in your
  documentation that you've used the IJG code. It would also be nice to mention
  that you use the Irrlicht Engine, the zlib and libPng. See the README files
  in the jpeglib, the zlib and libPng for further informations.
*/


#ifndef IRRRENDERER_H_INCLUDED
#define IRRRENDERER_H_INCLUDED

#include "CRenderer.h"
#include "ILightManagerCustom.h"
#include "CShaderLibrary.h"
#include "CMaterialSwapper.h"
#include "CPostProcessingEffectChain.h"
#include "CPostProcessingEffect.h"
#include "E_POST_PROCESSING_EFFECT.h"
#include "IShaderDefaultCallback.h"
#include "IShaderDefaultPostProcessCallback.h"
#include "MaterialCallbacks.h"
#include "SMaterials.h"

/*! \mainpage
\section intro_sec Introduction

Welcome to irrRenderer doxygen auto-generated documentation!
\section exmple_sec Example

\code
#include <irrlicht.h>
#include <irrRenderer.h>

int main()
{
    irr::IrrlichtDevice* device= createDevice(irr::video::EDT_OPENGL, irr::core::dimension2d<u32>(800,600));
    irr::video::CRenderer* renderer= createRenderer(device); //initialize irrRenderer

    device->getSceneManager()->loadScene("scene.irr");

    renderer->getMaterialSwapper()->swap(); //automatically swap the materials

    while(device->run())
    {
        device->getVideoDriver()->beginScene();
        renderer->drawAll();
        device->getVideoDriver()->endScene();
    }

    device->closeDevice();
    return 0;
}
\endcode


\section exmple_more RenderTest
For a more detailed example with source code, open the RenderTest project in RenderTest directory
*/

//! Use this function to create a new instance of the irrRenderer
/*! \param device irrlicht device to use
\param shaderDir directory which holds shader source files, relative to the executable
\return New instance of irrRenderer
*/
irr::video::CRenderer* createRenderer(irr::IrrlichtDevice* device, const irr::c8* shaderDir= "shaders/");

#endif // IRRRENDERER_H_INCLUDED
