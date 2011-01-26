#ifndef MATERIALCALLBACKS_H_INCLUDED
#define MATERIALCALLBACKS_H_INCLUDED

#include <irrlicht.h>
#include <iostream>

class DeferredComposeCallback : public irr::video::IShaderConstantSetCallBack
{
public:
    DeferredComposeCallback(scene::ISceneManager* smgr)
    {
        Smgr= smgr;
    }

    virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData)
    {
        int tex0= 0;
        int tex1= 1;
        services->setPixelShaderConstant("ColorTex", (float*)&tex0, 1);
        services->setPixelShaderConstant("NormalTex", (float*)&tex1, 1);

        core::matrix4 mat = services->getVideoDriver()->getTransform(video::ETS_PROJECTION);
        float farDist;
        if (mat[10]>0.f) farDist = -mat[14]/(mat[10]-1.f); // Left Handed
        else farDist = mat[14]/(mat[10]+1.f); // Right Handed
        services->setPixelShaderConstant("CamFar", &farDist, 1);
    }

private:
    scene::ISceneManager* Smgr;
};


class DefaultCallback : public irr::video::IShaderConstantSetCallBack
{
public:
    virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData)
    {
        core::matrix4 mat = services->getVideoDriver()->getTransform(video::ETS_PROJECTION);
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

        //!only for non-orthogonal
        core::matrix4 mat = services->getVideoDriver()->getTransform(video::ETS_PROJECTION);
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

#endif
