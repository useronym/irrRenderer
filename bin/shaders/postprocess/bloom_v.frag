#define QUALITY 3
#define _1QQUALITY 1/(QUALITY*QUALITY)
#define SIZE 2

uniform sampler2D Render;

uniform float PixelSizeX;
uniform float PixelSizeY;

vec2 clampCoord(in vec2 coord);

void main()
{
    vec4 sum = vec4(0.0);
    vec2 texcoord = vec2(gl_TexCoord[0]);
    vec4 baseColor= texture2D(Render, texcoord);
    float intensity= baseColor.r + baseColor.g + baseColor.b;
    intensity/= 3.0;
    intensity= 1.0 - intensity;

    for(int i= -QUALITY; i <= QUALITY; i++)
    {
        vec2 currentCoord= clampCoord(texcoord + vec2(0.0, PixelSizeY*i*SIZE));
        sum+= texture2D(Render, currentCoord) * _1QQUALITY;
    }

    baseColor+= sum;

    gl_FragColor= baseColor;
}

vec2 clampCoord(in vec2 coord)
{
    coord.x= clamp(coord.x, 0.0, 1.0);
    coord.y= clamp(coord.y, 0.0, 1.0);
    return coord;
}
