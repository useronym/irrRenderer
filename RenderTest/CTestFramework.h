#ifndef CTESTFRAMEWORK_H
#define CTESTFRAMEWORK_H

#include <irrlicht.h>
#include <irrRenderer.h>
#include <IrrlichtDeferredRendering.h>

using namespace irr;

class CTestFramework
{
    public:
        CTestFramework();

        ~CTestFramework();

        bool run();

    private:
        IrrlichtDevice* Device;
        video::CRenderer* Renderer;
};

#endif // CTESTFRAMEWORK_H
