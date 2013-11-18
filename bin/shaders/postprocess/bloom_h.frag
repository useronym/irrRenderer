#define KERNEL_SIZE 4 // actual kernel size = KERNEL_SIZE * 2 + 1, i.e. 7
#define STEP 1

uniform sampler2D Render;

uniform float PixelSizeX;
uniform float PixelSizeY;

float weights[] = {0.00390625, 0.03125, 0.109375, 0.21875, 0.2734375, 0.21875, 0.109375, 0.03125, 0.00390625};

vec2 clampCoord(in vec2 coord);
float luma(in vec4 color);

void main()
{
    vec4 bloom = vec4(0.0);
    vec2 texcoord = vec2(gl_TexCoord[0]);

    for(int i = -KERNEL_SIZE; i <= KERNEL_SIZE; i++)
    {
        vec2 cCoord = clampCoord(texcoord + vec2(PixelSizeX*i, 0.0));
        vec4 cColor = texture2D(Render, cCoord);
        bloom += cColor * luma(cColor) * weights[i + KERNEL_SIZE];
    }

    vec4 baseColor = texture2D(Render, texcoord);
    gl_FragColor = baseColor + bloom * (1.0 - luma(baseColor));
}

vec2 clampCoord(in vec2 coord)
{
    coord.x = clamp(coord.x, 0.0, 1.0);
    coord.y = clamp(coord.y, 0.0, 1.0);
    return coord;
}

float luma(in vec4 color)
{
    return 0.2126*color.r + 0.7152*color.g + 0.0722*color.b;
}
