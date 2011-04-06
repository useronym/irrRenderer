#define STRENGTH 4

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

    for(int i= -STRENGTH; i < STRENGTH; i++)
    {
        sum+= texture2D(Render, texcoord + vec2(PixelSizeX*i, PixelSizeY*i)) * intensity * (1.0/STRENGTH) * 0.5;
        sum+= texture2D(Render, texcoord + vec2(-PixelSizeX*i, PixelSizeY*i)) * intensity * (1.0/STRENGTH) * 0.5;
    }

    baseColor+= sum;

    gl_FragColor= baseColor;
}
