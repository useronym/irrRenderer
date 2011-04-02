#define STRENGTH 3

uniform sampler2D Render;

uniform float PixelSizeX;
uniform float PixelSizeY;

void main()
{
    vec4 sum = vec4(0);
    vec2 texcoord = vec2(gl_TexCoord[0]);
    vec4 baseColor= texture2D(Render, texcoord);
    int j;
    int i;

    for(i= -STRENGTH; i < STRENGTH; i++)
    {
        for (j= -STRENGTH-1; j < STRENGTH+1; j++)
        {
            sum+= texture2D(Render, texcoord + vec2(PixelSizeX*j, PixelSizeY*i)) * 0.15;
        }
    }

    if (baseColor.r < 0.3)
    {
        baseColor+= sum*sum*0.012;
    }
    else if (baseColor.r < 0.5)
    {
        baseColor+= sum*sum*0.009;
    }
    else
    {
        baseColor+= sum*sum*0.0075;
    }

    gl_FragColor= baseColor;
}
