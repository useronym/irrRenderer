#ifndef CTESTFRAMEWORK_H
#define CTESTFRAMEWORK_H

#include <irrlicht.h>
#include <irrRenderer.h>
#include <IrrlichtDeferredRendering.h>

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

        video::CPostProcessingEffect* AA, *Bloom;

        bool EventOccuredLastFrame;
};

#endif // CTESTFRAMEWORK_H
