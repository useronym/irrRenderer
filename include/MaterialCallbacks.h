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

class IShaderPointLightCallback : public irr::video::IShaderConstantSetCallBack
{
public:
    IShaderPointLightCallback(irr::scene::ISceneManager* smgr)
    {
        Smgr= smgr;
    }

    virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData)
    {
        int tex0= 0;
        int tex1= 1;
        int tex2= 2;
        services->setPixelShaderConstant("ColorTex", (float*)&tex0, 1);
        services->setPixelShaderConstant("NormalTex", (float*)&tex1, 1);
        services->setPixelShaderConstant("DepthTex", (float*)&tex2, 1);

        irr::scene::ICameraSceneNode* cam= Smgr->getActiveCamera();
        irr::core::matrix4 viewMat= Smgr->getVideoDriver()->getTransform(irr::video::ETS_VIEW);
        irr::core::matrix4 projMat = services->getVideoDriver()->getTransform(irr::video::ETS_PROJECTION);

        float farDist;
        if (projMat[10]>0.f) farDist = -projMat[14]/(projMat[10]-1.f); // Left Handed
        else farDist = projMat[14]/(projMat[10]+1.f); // Right Handed
        services->setPixelShaderConstant("CamFar", &farDist, 1);

        viewMat.transformVect(Pos);
        services->setPixelShaderConstant("Position", (float*)&Pos, 3);
        services->setPixelShaderConstant("Radius", &Radius, 1);
        services->setPixelShaderConstant("Color", (float*)&Color, 3);

        irr::core::vector3df farLeftUp= cam->getViewFrustum()->getFarLeftUp();
        viewMat.transformVect(farLeftUp);
        services->setVertexShaderConstant("VertexFarLeftUp", (float*)&farLeftUp, 3);
    }

    virtual void updateConstants(irr::video::SLight &light)
    {
        Pos= light.Position;
        Radius= light.Radius * 1.5;
        Color= light.DiffuseColor;
    }

private:
    irr::scene::ISceneManager* Smgr;
    irr::core::vector3df Pos;
    irr::video::SColorf Color;
    irr::f32 Radius;
};



class IShaderSpotLightCallback : public irr::video::IShaderConstantSetCallBack
{
public:
    IShaderSpotLightCallback(irr::scene::ISceneManager* smgr)
    {
        Smgr= smgr;
    }

    virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData)
    {
        int tex0= 0;
        int tex1= 1;
        int tex2= 2;
        services->setPixelShaderConstant("ColorTex", (float*)&tex0, 1);
        services->setPixelShaderConstant("NormalTex", (float*)&tex1, 1);
        services->setPixelShaderConstant("DepthTex", (float*)&tex2, 1);

        irr::scene::ICameraSceneNode* cam= Smgr->getActiveCamera();
        irr::core::matrix4 viewMat= Smgr->getVideoDriver()->getTransform(irr::video::ETS_VIEW);;
        irr::core::matrix4 projMat = services->getVideoDriver()->getTransform(irr::video::ETS_PROJECTION);

        float farDist;
        if (projMat[10]>0.f) farDist = -projMat[14]/(projMat[10]-1.f); // Left Handed
        else farDist = projMat[14]/(projMat[10]+1.f); // Right Handed
        services->setPixelShaderConstant("CamFar", &farDist, 1);

        viewMat.transformVect(Pos);
        viewMat.rotateVect(Direction);
        services->setPixelShaderConstant("Position", (float*)&Pos, 3);
        services->setPixelShaderConstant("Radius", &Radius, 1);
        services->setPixelShaderConstant("Color", (float*)&Color, 3);
        services->setPixelShaderConstant("Direction", (float*)&Direction, 3);
        services->setPixelShaderConstant("CosCutoff", (float*)&CosCutoff, 1);

        irr::core::vector3df farLeftUp= cam->getViewFrustum()->getFarLeftUp();
        viewMat.transformVect(farLeftUp);
        services->setVertexShaderConstant("VertexFarLeftUp", (float*)&farLeftUp, 3);
    }

    virtual void updateConstants(irr::video::SLight &light)
    {
        Pos= light.Position;
        Radius= light.Radius * 1.5;
        Color= light.DiffuseColor;
        Direction= light.Direction;
        CosCutoff= cos(light.OuterCone);

        /*     /\               Ga - Gamma
         *    /Ga\              Be - Beta
         *   /    \ B           B - Radius of the light
         *  /      \            C - Diameter of the cone
         * /Be______\
         *     C                Beta == (180 - Gamma)/2
         *
         *                      C/sin(Gamma) == B/sin(Beta)
         *                      C == (B/sin(Beta)) * sin(Gamma) //Nice and easy - the power of the Sines :)
        */
        ConeRadius= ((Radius / (sin((180.0 - light.OuterCone) / 2.0))) * sin(light.OuterCone)) / 2.0;
    }

    virtual irr::f32 getConeRadius()
    {
        return ConeRadius;
    }

private:
    irr::scene::ISceneManager* Smgr;
    irr::core::vector3df Pos;
    irr::video::SColorf Color;
    irr::core::vector3df Direction;
    irr::f32 Radius;
    irr::f32 CosCutoff;
    irr::f32 ConeRadius;
};



class IShaderDirectionalLightCallback : public irr::video::IShaderConstantSetCallBack
{
public:
    IShaderDirectionalLightCallback(irr::scene::ISceneManager* smgr)
    {
        Smgr= smgr;
    }

    virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData)
    {
        int tex0= 0;
        int tex1= 1;
        int tex2= 2;
        services->setPixelShaderConstant("ColorTex", (float*)&tex0, 1);
        services->setPixelShaderConstant("NormalTex", (float*)&tex1, 1);
        services->setPixelShaderConstant("DepthTex", (float*)&tex2, 1);

        irr::core::matrix4 viewMat= Smgr->getVideoDriver()->getTransform(irr::video::ETS_VIEW);
        viewMat.rotateVect(Direction);
        services->setPixelShaderConstant("Direction", (float*)&Direction, 3);

        services->setPixelShaderConstant("Color", (float*)&Color, 3);
    }

    virtual void updateConstants(irr::video::SLight &light)
    {
        Direction= light.Direction;
        Color= light.DiffuseColor;
    }

private:
    irr::scene::ISceneManager* Smgr;
    irr::core::vector3df Direction;
    irr::video::SColorf Color;
};



class IShaderAmbientLightCallback : public irr::video::IShaderConstantSetCallBack
{
public:
    IShaderAmbientLightCallback(irr::scene::ISceneManager* smgr)
    {
        Smgr= smgr;
    }

    virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData)
    {
        int tex0= 0;
        services->setPixelShaderConstant("ColorTex", (float*)&tex0, 1);
        int tex1= 1;
        services->setPixelShaderConstant("NormalTex", (float*)&tex1, 1);

        irr::core::vector3df ambientCol;
        ambientCol.X= Smgr->getAmbientLight().r;
        ambientCol.Y= Smgr->getAmbientLight().g;
        ambientCol.Z= Smgr->getAmbientLight().b;
        services->setPixelShaderConstant("Color", (float*)&ambientCol, 3);
    }

private:
    irr::scene::ISceneManager* Smgr;
};


class DefaultCallback : public irr::video::IShaderConstantSetCallBack
{
public:
    virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData)
    {
        int tex0= 0;
        int tex1= 1;
        services->setPixelShaderConstant("Tex0", (float*)&tex0, 1);
        services->setPixelShaderConstant("Tex1", (float*)&tex1, 1);

        irr::core::matrix4 mat = services->getVideoDriver()->getTransform(irr::video::ETS_PROJECTION);
        float farDist;
        if (mat[10]>0.f) farDist = -mat[14]/(mat[10]-1.f); // Left Handed
        else farDist = mat[14]/(mat[10]+1.f); // Right Handed
        services->setVertexShaderConstant("CamFar", &farDist, 1);

        services->setVertexShaderConstant("Repeat", &Repeat, 1);

        services->setVertexShaderConstant("Lighting", &Lighting, 1);
    }

    virtual void OnSetMaterial (const irr::video::SMaterial &material)
    {
        Repeat= material.MaterialTypeParam;
        if(Repeat < 1.0) Repeat= 1.0;

        Lighting= float(material.Lighting);
    }

private:
    float Repeat;
    float Lighting;
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
