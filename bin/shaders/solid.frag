varying vec3 Normal;
varying float Depth;

uniform float Lighting;
uniform sampler2D Tex0;

void main()
{
    vec3 vNormal= normalize(Normal);
    vNormal*= 0.5;
    vNormal+= 0.5;

    float vDepth= Depth;

    /*if(Lighting == 0.0)
    {
        vNormal= vec3(0.0, 0.0, 0.0);
        vDepth= 1.0;
    }*/


    gl_FragData[0] = texture2D(Tex0, gl_TexCoord[0].xy);
    // set normal to 0 for objects that should not receive lighting
    gl_FragData[1] = vec4(vNormal * Lighting, 0.0);
    gl_FragData[2] = vec4(vDepth, 0.0, 0.0, 0.0);
}
