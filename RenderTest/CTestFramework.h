#ifndef CTESTFRAMEWORK_H
#define CTESTFRAMEWORK_H

#include <irrlicht.h>
#include <irrRenderer.h>

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
        video::CPostProcessingEffect* AA, *Bloom;

        gui::IGUIStaticText* Console, *Help;
        bool DrawGBuffer, EventOccuredLastFrame;
};

#endif // CTESTFRAMEWORK_H
