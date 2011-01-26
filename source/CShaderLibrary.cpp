#include "CShaderLibrary.h"

CShaderLibrary::CShaderLibrary(char* shaderDir)
{
    ShaderDir= shaderDir;

    loadShader(deferred_compose_vs, "deferred_compose.vert");
    loadShader(deferred_compose_ps, "deferred_compose.frag");

    loadShader(terrain_vs, "terrain.vert");
    loadShader(terrain_ps, "terrain.frag");

    loadShader(solid_vs, "solid.vert");
    loadShader(solid_ps, "solid.frag");
}

void CShaderLibrary::loadShader(char* dest, char* fileSource)
{
    char fileName[128];
    strcpy(fileName, ShaderDir);
    strcat(fileName, fileSource);
    std::ifstream file;
    file.open(fileName);

    if(file.is_open())
    {
        char buff[128];
        file.getline(buff, 128);
        strcpy(dest, buff);
        strcat(dest, "\n");

        while(file.good())
        {
            char buff[128];
            file.getline(buff, 128);
            strcat(dest, buff);
            strcat(dest, "\n");
        }
    }
}
