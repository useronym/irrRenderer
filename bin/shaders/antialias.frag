uniform sampler2D Render;
uniform sampler2D MRT0; //depth

uniform float PixelSizeX;
uniform float PixelSizeY;

void main()
{
    vec2 coord= gl_TexCoord[0].xy;
    vec4 color= texture2D(Render, coord);
    float depth= texture2D(MRT0, coord).r;

    //check fo up/down edge
    float depthUpPrev= texture2D(MRT0, coord - vec2(0.0, PixelSizeY)).r;
    float depthUpNext= texture2D(MRT0, coord + vec2(0.0, PixelSizeY)).r;
    float upStepToPrev= abs(depth - depthUpPrev);
    float upStepToNext= abs(depth - depthUpNext);
    float upStepPrevToNext= abs(depthUpPrev - depthUpNext);
    if(upStepPrevToNext > 0.001 && abs(upStepToPrev - upStepToNext) > 0.001) color= vec4(0.8, 0.8, 0.8, 0.0);

    //check for left/right edge
    float depthRightPrev= texture2D(MRT0, coord - vec2(PixelSizeX, 0.0)).r;
    float depthRightNext= texture2D(MRT0, coord + vec2(PixelSizeX, 0.0)).r;
    float RightStepToPrev= abs(depth - depthRightPrev);
    float RightStepToNext= abs(depth - depthRightNext);
    float RightStepPrevToNext= abs(depthRightPrev - depthRightNext);
    if(RightStepPrevToNext > 0.001 && abs(RightStepToPrev - RightStepToNext) > 0.001) color= vec4(0.8, 0.8, 0.8, 0.0);

    gl_FragColor= color;
}
