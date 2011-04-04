#define STRENGTH 5

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
    intensity*= intensity;

    for(int i= -STRENGTH; i < STRENGTH; i++)
    {
        for (int j= -STRENGTH; j < STRENGTH; j++)
        {
            sum+= texture2D(Render, texcoord + vec2(PixelSizeX*j, PixelSizeY*i)) * intensity * 0.01;
        }
    }

    baseColor+= sum;

    gl_FragColor= baseColor;
}
