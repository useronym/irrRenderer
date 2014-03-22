#version 120
#extension GL_ARB_shading_language_420pack : enable

#define KERNEL_SIZE 4 // actual kernel size = KERNEL_SIZE * 2 + 1, i.e. 9

uniform sampler2D Render;

uniform float PixelSizeX;
uniform float PixelSizeY;

float weights[] = {0.00390625, 0.03125, 0.109375, 0.21875, 0.2734375, 0.21875, 0.109375, 0.03125, 0.00390625};

vec2 clampCoord(in vec2 coord);

void main()
{
    vec4 blur = vec4(0.0);
    vec2 texcoord = vec2(gl_TexCoord[0]);

    for(int i = -KERNEL_SIZE; i <= KERNEL_SIZE; i++)
    {
        vec2 cCoord = clampCoord(texcoord + vec2(0.0, PixelSizeY*i));
        blur += texture2D(Render, cCoord) * weights[i+KERNEL_SIZE];
    }

    gl_FragColor = blur;
}

vec2 clampCoord(in vec2 coord)
{
    coord.x = clamp(coord.x, 0.0, 1.0);
    coord.y = clamp(coord.y, 0.0, 1.0);
    return coord;
}
