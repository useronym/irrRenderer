#define QUALITY 3
#define SIZE 3.0

uniform sampler2D Render;

uniform float PixelSizeX;
uniform float PixelSizeY;

void main()
{
    vec4 sum = vec4(0.0);
    vec2 texcoord = vec2(gl_TexCoord[0]);
    vec4 baseColor= texture2D(Render, texcoord);
    float intensity= baseColor.r + baseColor.g + baseColor.b;
    intensity/= 3.0;
    intensity= 1.0 - intensity;
    intensity*= intensity * 0.5;

    for(int i= -QUALITY; i < QUALITY; i++)
    {
        sum+= texture2D(Render, texcoord + vec2(PixelSizeX*i*SIZE, 0.0)) * intensity * (1.0/QUALITY) * 0.3;
        sum+= texture2D(Render, texcoord + vec2(0.0, PixelSizeY*i*SIZE)) * intensity * (1.0/QUALITY) * 0.3;
        sum+= texture2D(Render, texcoord + vec2(PixelSizeX*i*SIZE, PixelSizeY*i*SIZE)) * intensity * (1.0/QUALITY) * 0.3;
        sum+= texture2D(Render, texcoord + vec2(-PixelSizeX*i*SIZE, PixelSizeY*i*SIZE)) * intensity * (1.0/QUALITY) * 0.3;
    }

    baseColor+= sum;

    gl_FragColor= baseColor;
}
