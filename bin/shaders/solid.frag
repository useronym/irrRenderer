varying vec3 Normal;
varying float Depth;

uniform sampler2D Tex0;

void main()
{
    vec3 vNormal= normalize(Normal);
    vNormal*= 0.5;
    vNormal+= 0.5;


    gl_FragData[0] = texture2D(Tex0, gl_TexCoord[0].xy);
    gl_FragData[1] = vec4(vNormal, 0.0);
    gl_FragData[2] = vec4(Depth, 0.0, 0.0, 0.0);
}
