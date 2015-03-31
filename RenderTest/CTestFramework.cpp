#include "CTestFramework.h"

CTestFramework::CTestFramework(bool vsync, bool automode)
    :RequestedExit(false),
     LastFPS(0),
     Vsync(vsync),
     Fullscreen(false),
     Resolution(800, 600)
{
    // autodetect video settings (resolution, depth)
    if(automode)
    {
        // create a NULL device to detect screen resolution
        IrrlichtDevice *nulldevice = createDevice(video::EDT_NULL);

        video::IVideoModeList* videoList = nulldevice->getVideoModeList();
        core::dimension2d<u32> screenRes = videoList->getDesktopResolution();
        Depth = videoList->getDesktopDepth();
        Resolution = videoList->getVideoModeResolution(core::dimension2d<u32>(0, 0), screenRes);
        Fullscreen = true;

        nulldevice -> drop();
    }

    Device = 0;
    Device = createDevice(video::EDT_OPENGL, Resolution, Depth, Fullscreen, false, Vsync, this);
    DrawGBuffer = false;
    timeLast = Device->getTimer()->getTime();
    timeSinceLastEvent = 500;

    // create a console
    Console = Device->getGUIEnvironment()->addStaticText(L"", core::rect<s32>(0,0, 800, 600));
    Console->setOverrideColor(video::SColor(255, 255, 255, 255));
    Console->setVisible(false);
    Device->getLogger()->log("GCC Version", __VERSION__);
    Device->getLogger()->log("Irrlicht Version", IRRLICHT_SDK_VERSION);
    Device->getLogger()->log("Vsync", Vsync == true ? "on" : "off");
    Device->getLogger()->log("Depth", core::stringc(Depth).c_str());
    core::stringc resolutionLog = automode == true ? "auto mode detected: " : "";
    resolutionLog += Resolution.Width;
    resolutionLog += " x ";
    resolutionLog += Resolution.Height;
    Device->getLogger()->log("Resolution", resolutionLog.c_str());
    Device->getLogger()->setLogLevel(ELL_INFORMATION);

    // set up the help thingy
    core::stringw helpText = L"IrrRenderer Demo\n\n";
    helpText += L"Key bindings:\n";
    helpText += L"Arrows - movement\n";
    helpText += L"Esc - toggle mouse capture\n";
    helpText += L"Space - solid / normal map / parallax map\n";
    helpText += L"F - toggle flashlight\n";
    helpText += L"G - show GBuffers debug info on screen\n";
    helpText += L"C - toggle console\n";
    helpText += L"H - toggle this help text\n";
    helpText += L"Q - quit\n";
    Help = Device->getGUIEnvironment()->addStaticText(helpText.c_str(),
            core::rect<s32>(core::vector2d<s32>(Resolution.Width-200, 75),
                            core::dimension2d<u32>(200, 400)));
    Help->setOverrideColor(video::SColor(255, 255, 255, 255));

    //! do the init
    Renderer = createRenderer(Device);

    //load teh scene
    scene::ISceneManager* smgr = Device->getSceneManager();
    smgr->loadScene("media/scene.irr");
    
    //! set automatically all materials in the current scene
    Renderer->getMaterialSwapper()->swapMaterials();

    // set up basic camera
    scene::ICameraSceneNode* cam = smgr->addCameraSceneNodeFPS(0, 100, 0.1);
    scene::ISceneNode* camPos = smgr->getSceneNodeFromName("camera");
    if(camPos)
    {
        cam->setPosition(camPos->getAbsolutePosition());
        camPos->remove();
    }
    // increase accuracy of depth buffer
    cam->setFarValue(1000);
    Device->getCursorControl()->setVisible(false);

    // set up a flashlight
    Flashlight = smgr->addLightSceneNode(cam);
    Flashlight->setLightType(video::ELT_SPOT);
    Flashlight->setVisible(false);
    video::SLight fl = Flashlight->getLightData();
    fl.OuterCone = 40;
    fl.Falloff = 2;
    fl.DiffuseColor.set(1.0, 0.8, 0.6);
    Flashlight->setLightData(fl);

    // set up walking beast
    scene::ISceneNode* beast = smgr->getSceneNodeFromName("beast");
    if(beast)
    {
        irr::scene::IAnimatedMeshSceneNode* animatedBeast = static_cast<irr::scene::IAnimatedMeshSceneNode*>(beast);
        animatedBeast->setFrameLoop(0,24);
        animatedBeast->setAnimationSpeed(25);
        // set the proper material
        beast->setMaterialType(Renderer->getMaterials()->NormalAnimated);
    }

    // set up dwarf
    scene::ISceneNode* dwarf = smgr->getSceneNodeFromName("dwarf");
    if(dwarf)
    {
        dwarf->setMaterialType(Renderer->getMaterials()->NormalAnimated);
    }

    // set up volumetric fog
    // this needs to be done because the .irr scene was exported with irrEdit that uses Irrlicht 1.5
    // serialization does not completely... work over there :)
    scene::ISceneNode *fog = smgr->getSceneNodeFromName("fog");
    if (fog)
    {
        fog->setMaterialType(Renderer->getMaterials()->TransparentSoft);

        scene::IParticleEmitter* emit = static_cast<scene::IParticleSystemSceneNode*>(fog)->getEmitter();
        emit->setMinStartSize(core::dimension2df(30, 30));
        emit->setMaxStartSize(core::dimension2df(50, 50));
    }

    // set up tangent meshes
    core::array<scene::ISceneNode*> nodes;
    Device->getSceneManager()->getSceneNodesFromType(scene::ESNT_MESH, nodes);
    for(u32 i = 0; i < nodes.size(); i++)
    {
        scene::IMeshSceneNode* mnode = static_cast<scene::IMeshSceneNode*>(nodes[i]);
        scene::IMesh* tangentMesh = 0;
        tangentMesh = smgr->getMeshManipulator()->createMeshWithTangents(mnode->getMesh());

        mnode = 0;
        mnode = smgr->addMeshSceneNode(tangentMesh);
        if(mnode)
        {
            mnode->setPosition(nodes[i]->getPosition());
            mnode->setRotation(nodes[i]->getRotation());
            mnode->setScale(nodes[i]->getScale());

            for(u32 ii = 0; ii < nodes[i]->getMaterialCount(); ii++)
            {
                mnode->getMaterial(ii) = nodes[i]->getMaterial(ii);
            }

            //! set the proper material
            mnode->setMaterialType(Renderer->getMaterials()->Parallax);
        }

        nodes[i]->remove();
        if(tangentMesh)tangentMesh->drop();
    }


    Device->getLogger()->log("Who's that callin'?"); //Ain't nobody there
}

CTestFramework::~CTestFramework()
{
    delete Renderer;
    Device->closeDevice();
}

bool CTestFramework::run()
{
    timeLast = Device->getTimer()->getTime();

    Device->getVideoDriver()->beginScene();
    Device->getSceneManager()->drawAll();

    u32 fps = Device->getVideoDriver()->getFPS();
    if (fps != LastFPS)
    {
        core::stringw str = L"irrRenderer - RenderTest";
        str += L" | FPS: ";
        str += fps;
        str += L" | ms/frame: ";
        str += (1.0 / Device->getVideoDriver()->getFPS())*1000.0;
        str += L" | Tris: ";
        str += Device->getVideoDriver()->getPrimitiveCountDrawn();
        Device->setWindowCaption(str.c_str());
        LastFPS = fps;
    }

    //! draw GBuffer debug info
    if(DrawGBuffer)
    {
        irr::core::recti gbuffRect= irr::core::recti(irr::core::vector2di(0, 0),Renderer->getMRT(0)->getSize());
        for(irr::u32 i = 0; i < Renderer->getMRTCount(); i++)
        {
            irr::core::recti gbuffRectSmall = irr::core::recti(irr::core::vector2di(Renderer->getMRT(i)->getSize().Width/4.0*i, 0), irr::core::vector2di(Renderer->getMRT(i)->getSize().Width/4.0*(i+1), Renderer->getMRT(i)->getSize().Height/4.0));
            Device->getVideoDriver()->draw2DImage(Renderer->getMRT(i), gbuffRectSmall, gbuffRect);
        }
    }

    Device->getVideoDriver()->setMaterial(video::SMaterial());
    Console->draw();
    Help->draw();

    Device->getVideoDriver()->endScene();
    return (Device->run() && !RequestedExit);
}

bool CTestFramework::OnEvent(const SEvent& event)
{
    if(Device)
    {
        scene::ICameraSceneNode* cam = static_cast<scene::ICameraSceneNode*>(Device->getSceneManager()->getSceneNodeFromType(scene::ESNT_CAMERA));
        if(cam)cam->OnEvent(event);

        if(event.EventType == irr::EET_KEY_INPUT_EVENT && !event.KeyInput.PressedDown)
        {
            if(event.KeyInput.Key == KEY_ESCAPE) // release mouse
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
                //AA->setActive(!AA->isActive());
            }
            else if(event.KeyInput.Key == KEY_KEY_B)
            {
                //Bloom->setActive(!Bloom->isActive());
            }
            else if(event.KeyInput.Key == KEY_KEY_G)
            {
                DrawGBuffer = !DrawGBuffer;
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
                screenName += ".png";
                Device->getVideoDriver()->writeImageToFile(Device->getVideoDriver()->createScreenShot(), screenName.c_str());
            }
            else if(event.KeyInput.Key == KEY_KEY_Q)
            {
                RequestedExit = true;
            }

            timeSinceLastEvent = 0;
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
            timeSinceLastEvent += Device->getTimer()->getTime() - timeLast;
        }
    }

    return false;
}
