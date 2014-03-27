//This is kinda funky

#include "CTestFramework.h"
#include <iostream>
#include <string>

using namespace std;

int main(int argc,char *argv[])
{
    bool vsync = false;
    bool automode = false;

    for(unsigned int i = 0; i < argc; i++)
    {
        std::string s = argv[i];

        if(s == "-h" || s == "--help")
        {
            std::cout << "RenderTest, an irrRenderer demo." << std::endl;
            std::cout << "Compiled with Irrlicht " << IRRLICHT_SDK_VERSION;
            #ifdef __GNUC__
                std::cout << ", GCC " << __VERSION__ << std::endl;
            #endif // __GNUC__
            std::cout << "Arguments:" << std::endl;
            std::cout << "\t-v --vsync\n" << "\t\t Start with vertical synchronization enabled\n";
            std::cout << "\t-a --auto\n" << "\t\t Start in full screen mode, autodetect desktop resolution\n";
            std::cout << "\t-h --help\n" << "\t\t Print this message\n";

            return 0;
        }
        else if(s == "--vsync" || s == "-v")
        {
            vsync = true;
        }
        else if(s == "--auto" || s == "-a")
        {
            automode = true;
        }
    }

    CTestFramework* test= new CTestFramework(vsync, automode);
    while(test->run())
    {
        //funky,
        //funky,
        //funky!
    }

    delete test;
    return 0;
}
