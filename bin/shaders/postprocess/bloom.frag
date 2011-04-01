#define STRENGTH 3

uniform sampler2D Render;

uniform float PixelSizeX;
uniform float PixelSizeY;

void main()
{
    vec4 sum = vec4(0);
    vec2 texcoord = vec2(gl_TexCoord[0]);
    int j;
    int i;

    for(i= -STRENGTH; i < STRENGTH; i++)
    {
        for (j= -STRENGTH; j < STRENGTH; j++)
        {
            sum+= texture2D(Render, texcoord + vec2(PixelSizeX*j, PixelSizeY*i)) * 0.03;
        }
    }

    gl_FragColor= sum + texture2D(Render, texcoord);
}
