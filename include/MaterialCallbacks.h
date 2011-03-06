// This file is part of the "irrRenderer".
// For conditions of distribution and use, see copyright notice in irrRenderer.h

#ifndef MATERIALCALLBACKS_H_INCLUDED
#define MATERIALCALLBACKS_H_INCLUDED

#include <irrlicht.h>
#include <iostream>


namespace irr
{
namespace video
{

class DeferredComposeCallback : public irr::video::IShaderConstantSetCallBack
{
public:
    DeferredComposeCallback(irr::scene::ISceneManager* smgr)
    {
        Smgr= smgr;
    }

    virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData)
    {
        int tex0= 0;
        int tex1= 1;
        services->setPixelShaderConstant("ColorTex", (float*)&tex0, 1);
        services->setPixelShaderConstant("NormalTex", (float*)&tex1, 1);

        irr::scene::ICameraSceneNode* cam= Smgr->getActiveCamera();
        irr::core::matrix4 viewMat= Smgr->getVideoDriver()->getTransform(irr::video::ETS_VIEW);;
        irr::core::vector3df farLeftUp= cam->getViewFrustum()->getFarLeftUp();
        viewMat.transformVect(farLeftUp);
        services->setVertexShaderConstant("VertexFarLeftUp", (float*)&farLeftUp, 3);
    }

private:
    irr::scene::ISceneManager* Smgr;
};

class IShaderLightCallback : public irr::video::IShaderConstantSetCallBack
{
public:
    IShaderLightCallback(irr::scene::ISceneManager* smgr)
    {
        Smgr= smgr;
    }

    virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData)
    {
        int tex0= 0;
        int tex1= 1;
        services->setPixelShaderConstant("ColorTex", (float*)&tex0, 1);
        services->setPixelShaderConstant("NormalTex", (float*)&tex1, 1);

        irr::scene::ICameraSceneNode* cam= Smgr->getActiveCamera();
        irr::core::matrix4 viewMat= Smgr->getVideoDriver()->getTransform(irr::video::ETS_VIEW);;

        viewMat.transformVect(Pos);
        services->setPixelShaderConstant("Position", (float*)&Pos, 3);
        services->setPixelShaderConstant("Radius", &Radius, 1);

        irr::core::vector3df farLeftUp= cam->getViewFrustum()->getFarLeftUp();
        viewMat.transformVect(farLeftUp);
        services->setVertexShaderConstant("VertexFarLeftUp", (float*)&farLeftUp, 3);

        irr::core::matrix4 mat = services->getVideoDriver()->getTransform(irr::video::ETS_PROJECTION);
        float farDist;
        if (mat[10]>0.f) farDist = -mat[14]/(mat[10]-1.f); // Left Handed
        else farDist = mat[14]/(mat[10]+1.f); // Right Handed
        services->setPixelShaderConstant("VertexCamFar", &farDist, 1);
    }

    virtual void updateConstants(irr::video::SLight &light)
    {
        Pos= light.Position;
        Radius= light.Radius;
    }

private:
    irr::scene::ISceneManager* Smgr;
    irr::core::vector3df Pos;
    irr::f32 Radius;
};


class DefaultCallback : public irr::video::IShaderConstantSetCallBack
{
public:
    virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData)
    {
        irr::core::matrix4 mat = services->getVideoDriver()->getTransform(irr::video::ETS_PROJECTION);
        float farDist;
        if (mat[10]>0.f) farDist = -mat[14]/(mat[10]-1.f); // Left Handed
        else farDist = mat[14]/(mat[10]+1.f); // Right Handed
        services->setVertexShaderConstant("CamFar", &farDist, 1);

        services->setVertexShaderConstant("Repeat", &Repeat, 1);
    }

    virtual void OnSetMaterial (const irr::video::SMaterial &material)
    {
        Repeat= material.MaterialTypeParam;
        if(Repeat < 1.0) Repeat= 1.0;
    }

private:
    float Repeat;
};


class TerrainCallback : public irr::video::IShaderConstantSetCallBack
{
public:
    virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData)
    {
        int tex0= 0;
        int tex1= 1;
        int tex2= 2;
        int tex3= 3;
        services->setPixelShaderConstant("Tex0", (float*)&tex0, 1);
        services->setPixelShaderConstant("Tex1", (float*)&tex1, 1);
        services->setPixelShaderConstant("Tex2", (float*)&tex2, 1);
        services->setPixelShaderConstant("Tex3", (float*)&tex3, 1);

        irr::core::matrix4 mat = services->getVideoDriver()->getTransform(irr::video::ETS_PROJECTION);
        float farDist;
        if (mat[10]>0.f) farDist = -mat[14]/(mat[10]-1.f); // Left Handed
        else farDist = mat[14]/(mat[10]+1.f); // Right Handed
        services->setVertexShaderConstant("CamFar", &farDist, 1);

        services->setVertexShaderConstant("Repeat", &Repeat, 1);
    }

    virtual void OnSetMaterial (const irr::video::SMaterial &material)
    {
        Repeat= material.MaterialTypeParam;
        if(Repeat < 1.0) Repeat= 1.0;
    }

private:
    float Repeat;
};

}
}


#endif
