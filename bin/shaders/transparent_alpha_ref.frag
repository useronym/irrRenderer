varying vec3 Normal;
varying float Depth;

uniform sampler2D Tex0;
uniform float Lighting;

void main()
{
    vec4 color= texture2D(Tex0, gl_TexCoord[0].xy);
    if(color.a < 0.5) discard; //a transparent pixel

    vec3 vNormal;
    float vDepth;

    if(Lighting < 0.5)
    {
        vNormal= vec3(0.0);
        vDepth= 1.0;
    }
    else
    {
        vNormal = normalize(Normal);
        vNormal *= 0.5;
        vNormal += 0.5;
        vDepth = Depth;
    }

    gl_FragData[0] = color;
    gl_FragData[1] = vec4(vNormal * Lighting, 0.0);
    gl_FragData[2] = vec4(vDepth, 0.0, 0.0, 0.0);
}

