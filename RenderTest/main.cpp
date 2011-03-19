//This is kinda funky

#include "CTestFramework.h"

using namespace std;

int main()
{
    CTestFramework* test= new CTestFramework;
    while(test->run())
    {
        //funky,
        //funky,
        //funky!
    }

    delete test;
    return 0;
}
