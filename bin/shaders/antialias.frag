uniform sampler2D Render;
uniform sampler2D MRT0; //depth

uniform float PixelSizeX;
uniform float PixelSizeY;

void main()
{
    vec2 coord= gl_TexCoord[0].xy;
    vec4 color= texture2D(Render, coord);
    float depth= texture2D(MRT0, coord).r;

    float lefttoRight= abs(texture2D(MRT0, coord + vec2(-PixelSizeX, 0.0)).r - depth) +
                       abs(texture2D(MRT0, coord + vec2(PixelSizeX, 0.0)).r - depth);
    if(lefttoRight > 0.01) color= (texture2D(Render, coord + vec2(-PixelSizeX, 0.0)) +
                                  texture2D(Render, coord + vec2(PixelSizeX, 0.0))) / 2.0;

    gl_FragColor= color;
}
