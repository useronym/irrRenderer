#ifndef CSHADERS_H
#define CSHADERS_H

#include <fstream>
#include <string.h>
#include <iostream>

class CShaderLibrary
{
    public:
        CShaderLibrary(char* shaderDir);

        char deferred_compose_vs[1024];
        char deferred_compose_ps[1024];

        char solid_vs[1024];
        char solid_ps[1024];

        char terrain_vs[1024];
        char terrain_ps[1024];

    private:
        char* ShaderDir;
        void loadShader(char* dest, char* fileSource);
};

#endif
