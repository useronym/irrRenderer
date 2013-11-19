#include "CTestFramework.h"

CTestFramework::CTestFramework()
{
    Device= 0;
    Device= createDevice(video::EDT_OPENGL, core::dimension2d<u32>(800,600), 16, false, false, false, this);
    DrawGBuffer= false;
    timeLast= Device->getTimer()->getTime();
    timeSinceLastEvent= 500;

    //create a console
    Console= Device->getGUIEnvironment()->addStaticText(L"", core::rect<s32>(0,0, 800, 600));
    Console->setOverrideColor(video::SColor(255, 255, 255, 255));
    Console->setVisible(false);

    //set up the help thingy
    core::stringw helpText= L"IrrRenderer Demo\n\n";
    helpText+= L"Key bindings:\n";
    helpText+= L"Arrows - movement\n";
    helpText+= L"Esc - toggle mouse capture\n";
    helpText+= L"A - toggle antialiasing\n";
    helpText+= L"B - toggle bloom\n";
    helpText+= L"Space - switch between solid, normal map and parallax map materials\n";
    helpText+= L"F - toggle flashlight\n";
    helpText+= L"G - show GBuffers debug info on screen\n";
    helpText+= L"C - toggle console\n";
    helpText+= L"H - toggle this help text\n";
    Help= Device->getGUIEnvironment()->addStaticText(helpText.c_str(), core::rect<s32>(600, 10, 800, 600));
    Help->setOverrideColor(video::SColor(255, 255, 255, 255));

    //!important do the init
    Renderer= createRenderer(Device);

    //load teh scene
    scene::ISceneManager* smgr= Device->getSceneManager();
    smgr->loadScene("media/scene.irr");

    //set up basic camera
    scene::ICameraSceneNode* cam= smgr->addCameraSceneNodeFPS(0, 100, 0.1);
    scene::ISceneNode* camPos= smgr->getSceneNodeFromName("camera");
    if(camPos)
    {
        cam->setPosition(camPos->getAbsolutePosition());
        camPos->remove();
    }
    cam->setFarValue(1000); //to increase accuracy of depth buffer
    Device->getCursorControl()->setVisible(false);

    //set up a flashlight
    Flashlight= smgr->addLightSceneNode(cam);
    Flashlight->setLightType(video::ELT_SPOT);
    Flashlight->setVisible(false);
    video::SLight fl= Flashlight->getLightData();
    fl.OuterCone= 40;
    fl.Falloff= 2;
    fl.DiffuseColor.set(1.0, 1.0, 0.5);
    Flashlight->setLightData(fl);

    //set up walking beast
    scene::ISceneNode* beast= smgr->getSceneNodeFromName("beast");
    if(beast)
    {
        if(beast->getType() == irr::scene::ESNT_ANIMATED_MESH)
        {
            irr::scene::IAnimatedMeshSceneNode* animatedBeast= static_cast<irr::scene::IAnimatedMeshSceneNode*>(beast);
            animatedBeast->setFrameLoop(0,24);
            animatedBeast->setAnimationSpeed(25);
        }
    }

    //set up tangent meshes
    core::array<scene::ISceneNode*> nodes;
    Device->getSceneManager()->getSceneNodesFromType(scene::ESNT_MESH, nodes);
    for(u32 i= 0; i < nodes.size(); i++)
    {
        scene::IMeshSceneNode* mnode= static_cast<scene::IMeshSceneNode*>(nodes[i]);
        scene::IMesh* tangentMesh= 0;
        tangentMesh= smgr->getMeshManipulator()->createMeshWithTangents(mnode->getMesh());

        mnode= 0;
        mnode= smgr->addMeshSceneNode(tangentMesh);
        if(mnode)
        {
            mnode->setPosition(nodes[i]->getPosition());
            mnode->setRotation(nodes[i]->getRotation());
            mnode->setScale(nodes[i]->getScale());

            for(u32 ii= 0; ii < nodes[i]->getMaterialCount(); ii++)
            {
                mnode->getMaterial(ii)= nodes[i]->getMaterial(ii);
            }

            //!important set the proper material
            mnode->setMaterialType(Renderer->getMaterials()->Parallax);
        }

        nodes[i]->remove();
        if(tangentMesh)tangentMesh->drop();
    }

    //!important set automatically all materials
    irr::video::CMaterialSwapper* swapper= Renderer->getMaterialSwapper();
    swapper->swapMaterials();

    //!important set up post processing
    Renderer->createPostProcessingEffect(irr::video::EPE_FOG);
    AA= Renderer->createPostProcessingEffect(irr::video::EPE_ANTIALIASING);

    Bloom= Renderer->createPostProcessingEffectChain();
    Bloom->createEffect(irr::video::EPE_BLOOM_V); //Vertical bloom
    //Bloom->createEffect(irr::video::EPE_BLOOM_H); //Horizontal bloom

    AA->setActive(false);
    Bloom->setActive(false);


    Device->getLogger()->log("Who's that callin'?"); //Ain't nobody there
}

CTestFramework::~CTestFramework()
{
    delete Renderer;
    Device->closeDevice();
}

bool CTestFramework::run()
{
    timeLast= Device->getTimer()->getTime();

    Device->getVideoDriver()->beginScene();
    Device->getSceneManager()->drawAll();
    core::stringw str= L"irrRenderer - RenderTest";
    str+= L" | FPS: ";
    str+= Device->getVideoDriver()->getFPS();
    str+= L" | ms/frame: ";
    str+= (1.0 / Device->getVideoDriver()->getFPS())*1000.0;
    str+= L" | Tris: ";
    str+= Device->getVideoDriver()->getPrimitiveCountDrawn();
    Device->setWindowCaption(str.c_str());

    //!important (but only for someone) draw GBuffer debug info
    if(DrawGBuffer)
    {
        irr::core::recti gbuffRect= irr::core::recti(irr::core::vector2di(0, 0),Renderer->getMRT(0)->getSize());
        for(irr::u32 i= 0; i < Renderer->getMRTCount(); i++)
        {
            irr::core::recti gbuffRectSmall= irr::core::recti(irr::core::vector2di(Renderer->getMRT(i)->getSize().Width/4.0*i, 0), irr::core::vector2di(Renderer->getMRT(i)->getSize().Width/4.0*(i+1), Renderer->getMRT(i)->getSize().Height/4.0));
            Device->getVideoDriver()->draw2DImage(Renderer->getMRT(i), gbuffRectSmall, gbuffRect);
        }
    }

    Device->getVideoDriver()->setMaterial(video::SMaterial());
    Console->draw();
    Help->draw();

    Device->getVideoDriver()->endScene();
    return Device->run();
}

bool CTestFramework::OnEvent(const SEvent& event)
{
    if(Device)
    {
        scene::ICameraSceneNode* cam = static_cast<scene::ICameraSceneNode*>(Device->getSceneManager()->getSceneNodeFromType(scene::ESNT_CAMERA));
        if(cam)cam->OnEvent(event);

        if(event.EventType == irr::EET_KEY_INPUT_EVENT && !event.KeyInput.PressedDown)
        {
            if(event.KeyInput.Key == KEY_ESCAPE) //release mouse
            {
                cam->setInputReceiverEnabled(!cam->isInputReceiverEnabled());
                Device->getCursorControl()->setVisible(!cam->isInputReceiverEnabled());
            }
            else if(event.KeyInput.Key == KEY_SPACE)
            {
                core::array<scene::ISceneNode*> meshNodes;
                Device->getSceneManager()->getSceneNodesFromType(scene::ESNT_MESH, meshNodes);
                for(u32 i = 0; i < meshNodes.size(); i++)
                {
                    if(meshNodes[i]->getMaterial(0).MaterialType == Renderer->getMaterials()->Solid) meshNodes[i]->setMaterialType(Renderer->getMaterials()->Normal);
                    else if(meshNodes[i]->getMaterial(0).MaterialType == Renderer->getMaterials()->Normal) meshNodes[i]->setMaterialType(Renderer->getMaterials()->Parallax);
                    else if(meshNodes[i]->getMaterial(0).MaterialType == Renderer->getMaterials()->Parallax) meshNodes[i]->setMaterialType(Renderer->getMaterials()->Solid);
                }
            }
            else if(event.KeyInput.Key == KEY_KEY_A)
            {
                AA->setActive(!AA->isActive());
            }
            else if(event.KeyInput.Key == KEY_KEY_B)
            {
                Bloom->setActive(!Bloom->isActive());
            }
            else if(event.KeyInput.Key == KEY_KEY_G)
            {
                DrawGBuffer= !DrawGBuffer;
            }
            else if(event.KeyInput.Key == KEY_KEY_C)
            {
                Console->setVisible(!Console->isVisible());
            }
            else if(event.KeyInput.Key == KEY_KEY_H)
            {
                Help->setVisible(!Help->isVisible());
            }
            else if(event.KeyInput.Key == KEY_KEY_F)
            {
                Flashlight->setVisible(!Flashlight->isVisible());
            }
            else if(event.KeyInput.Key == KEY_F12)
            {
                irr::core::stringc screenName = irr::core::stringc(Device->getTimer()->getRealTime());
                screenName += ".jpg";
                Device->getVideoDriver()->writeImageToFile(Device->getVideoDriver()->createScreenShot(), screenName.c_str());
            }

            timeSinceLastEvent= 0;
        }
        else if(event.EventType == EET_LOG_TEXT_EVENT)
        {
            core::stringw text = Console->getText();
            text += event.LogEvent.Text;
            text += L"\n";
            Console->setText(text.c_str());
        }
        else
        {
            timeSinceLastEvent+= Device->getTimer()->getTime() - timeLast;
        }
    }

    return false;
}
