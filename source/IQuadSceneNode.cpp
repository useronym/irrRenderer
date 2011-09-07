#include "IQuadSceneNode.h"
irr::scene::IQuadSceneNode::IQuadSceneNode(irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id) : irr::scene::ISceneNode(parent, mgr)
{
    //remove();

    Buffer = new irr::scene::SMeshBuffer;
    Buffer->Vertices.push_back(irr::video::S3DVertex(
                                   irr::core::vector3df(-1.0, 1.0, 0.0),
                                   irr::core::vector3df(0.0, 0.0, 1.0), irr::video::SColor(255,255,255,255),
                                   irr::core::vector2df(0.0, 0.0)));
    Buffer->Vertices.push_back(irr::video::S3DVertex(
                                   irr::core::vector3df(1.0, 1.0, 0.0),
                                   irr::core::vector3df(0.0, 0.0, 1.0), irr::video::SColor(255,255,255,255),
                                   irr::core::vector2df(1.0, 0.0)));
    Buffer->Vertices.push_back(irr::video::S3DVertex(
                                   irr::core::vector3df(1.0, -1.0, 0.0),
                                   irr::core::vector3df(0.0, 0.0, 1.0), irr::video::SColor(255,255,255,255),
                                   irr::core::vector2df(1.0, 1.0)));
    Buffer->Vertices.push_back(irr::video::S3DVertex(
                                   irr::core::vector3df(-1.0, -1.0, 0.0),
                                   irr::core::vector3df(0.0, 0.0, 1.0), irr::video::SColor(255,255,255,255),
                                   irr::core::vector2df(0.0, 1.0)));

    Buffer->Indices.push_back(0);
    Buffer->Indices.push_back(1);
    Buffer->Indices.push_back(2);
    Buffer->Indices.push_back(3);
    Buffer->Indices.push_back(0);
    Buffer->Indices.push_back(2);
    Buffer->recalculateBoundingBox();
    //setScale(irr::core::vector3df(20,20,20));
    updateAbsolutePosition();
    Material.ZBuffer = false;
    Material.ZWriteEnable = false;
}

irr::scene::IQuadSceneNode::~IQuadSceneNode()
{
    delete Buffer;
}

void irr::scene::IQuadSceneNode::render()
{
    irr::video::IVideoDriver* driver = SceneManager->getVideoDriver();
    driver->setTransform(irr::video::ETS_WORLD, AbsoluteTransformation);
    driver->setMaterial(Material);
    driver->drawMeshBuffer(Buffer);
}

void irr::scene::IQuadSceneNode::OnRegisterSceneNode()
{

}

void irr::scene::IQuadSceneNode::setMaterialTexture(irr::u32 layer, irr::video::ITexture* texture)
{
    Material.setTexture(layer, texture);
}

void irr::scene::IQuadSceneNode::setMaterialType(irr::video::E_MATERIAL_TYPE tehType)
{
    Material.MaterialType= tehType;
}

const irr::core::aabbox3d<irr::f32>& irr::scene::IQuadSceneNode::getBoundingBox() const
{
    return Buffer->getBoundingBox();
}

irr::video::SMaterial& irr::scene::IQuadSceneNode::getMaterial(irr::u32 num)
{
    return Material;
}

irr::u32 irr::scene::IQuadSceneNode::getMaterialCount()
{
    return 1;
}
