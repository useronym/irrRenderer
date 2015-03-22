uniform sampler2D Tex0;
uniform sampler2D Tex1;

#define FACTOR 50.0

varying float Depth;

void main()
{
    vec2 coord = gl_FragCoord.xy / vec2(800.0, 600.0);
    float backgroundDepth = texture2D(Tex1, coord).r;
    if (Depth > backgroundDepth)
        discard;

    float diff = backgroundDepth - Depth;
    float softness = clamp(FACTOR * diff, 0.0, 1.0);

    gl_FragColor = texture2D(Tex0, gl_TexCoord[0].xy) * vec4(1.0, 1.0, 1.0, softness);
}

