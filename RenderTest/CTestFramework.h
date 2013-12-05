#ifndef CTESTFRAMEWORK_H
#define CTESTFRAMEWORK_H

#include <irrlicht.h>
#include <irrRenderer.h>
#include <iostream>

using namespace irr;

class CTestFramework : public IEventReceiver
{
    public:
        CTestFramework(bool vsync, bool automode);

        ~CTestFramework();

        bool run();

        virtual bool OnEvent(const SEvent& event);

    private:
        IrrlichtDevice* Device;
        video::CRenderer* Renderer;

        bool Vsync, Fullscreen;
        u32 Depth;
        core::dimension2d<u32> Resolution;

        scene::ILightSceneNode* Flashlight;
        video::CPostProcessingEffect* AA;
        video::CPostProcessingEffectChain* Bloom;

        gui::IGUIStaticText* Console, *Help;
        bool DrawGBuffer;
        irr::u32 timeLast, timeSinceLastEvent;
};

#endif // CTESTFRAMEWORK_H
