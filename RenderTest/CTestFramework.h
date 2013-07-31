#ifndef CTESTFRAMEWORK_H
#define CTESTFRAMEWORK_H

#include <irrlicht.h>
#include <irrRenderer.h>
#include <iostream>

using namespace irr;

class CTestFramework : public IEventReceiver
{
    public:
        CTestFramework();

        ~CTestFramework();

        bool run();

        virtual bool OnEvent(const SEvent& event);

    private:
        IrrlichtDevice* Device;
        video::CRenderer* Renderer;

        scene::ILightSceneNode* Flashlight;
        video::CPostProcessingEffect* AA;
        video::CPostProcessingEffectChain* Bloom;

        gui::IGUIStaticText* Console, *Help;
        bool DrawGBuffer;
        irr::u32 timeLast, timeSinceLastEvent;
};

#endif // CTESTFRAMEWORK_H
