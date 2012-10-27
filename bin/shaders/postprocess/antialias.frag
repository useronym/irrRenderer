uniform sampler2D Render;
uniform sampler2D Tex0; //depth

uniform float PixelSizeX;
uniform float PixelSizeY;

vec4 blurPixel(in vec4 pixelColor, in vec2 pixelCoord);

void main()
{
    vec2 coord= gl_TexCoord[0].xy;
    vec4 color= texture2D(Render, coord);

    //hey whatcha doin, hey don't check screen edges
    coord.x= clamp(coord.x, PixelSizeX*2.0, 1.0 - (PixelSizeX*2.0));
    coord.y= clamp(coord.y, PixelSizeY*2.0, 1.0 - (PixelSizeY*2.0));

    float depth= texture2D(Tex0, coord).r;
    float depthUp= texture2D(Tex0, coord + vec2(0.0, -PixelSizeY)).r;
    float depthDown= texture2D(Tex0, coord + vec2(0.0, PixelSizeY)).r;
    float depthLeft= texture2D(Tex0, coord + vec2(-PixelSizeX, 0.0)).r;
    float depthRight= texture2D(Tex0, coord + vec2(PixelSizeX, 0.0)).r;

    float depthUpLeft= texture2D(Tex0, coord + vec2(-PixelSizeX, -PixelSizeY)).r;
    float depthDownRight= texture2D(Tex0, coord + vec2(PixelSizeX, PixelSizeY)).r;
    float depthUpRight= texture2D(Tex0, coord + vec2(PixelSizeX, -PixelSizeY)).r;
    float depthDownLeft= texture2D(Tex0, coord + vec2(-PixelSizeX, PixelSizeY)).r;

    if( abs(((depthUp + depthDown + depthLeft + depthRight + depthUpLeft + depthDownRight + depthUpRight + depthDownLeft) / 8.0) - depth) > depth/1250.0)
    {
        color= blurPixel(color, coord);
        //color= vec4(0.9, 0.9, 0.9, 0.0);
    }

    gl_FragColor= color;
}

vec4 blurPixel(in vec4 pixelColor, in vec2 pixelCoord)
{
    pixelColor+= texture2D(Render, pixelCoord + vec2(-PixelSizeX, 0.0));
    pixelColor+= texture2D(Render, pixelCoord + vec2(PixelSizeX, 0.0));
    pixelColor+= texture2D(Render, pixelCoord + vec2(0.0, -PixelSizeY));
    pixelColor+= texture2D(Render, pixelCoord + vec2(0.0, PixelSizeY));
    pixelColor/= 5.0;
    return pixelColor;
}
