varying vec3 Normal;
varying vec3 Tangent;
varying vec3 Binormal;

varying float Depth;

uniform sampler2D Tex0;
uniform sampler2D Tex1;

void main()
{
    vec3 normalFromTex = ((texture2D(Tex1, gl_TexCoord[0].xy) * 2.0) - 1.0).xyz;
    mat3 tangentToView = mat3(Tangent, Binormal, Normal);
    normalFromTex = normalize(tangentToView * normalFromTex);

    normalFromTex *= 0.5;
    normalFromTex += 0.5;

    gl_FragData[0] = texture2D(Tex0, gl_TexCoord[0].xy);
    gl_FragData[1] = vec4(normalFromTex, 0.0);
    gl_FragData[2] = vec4(Depth, 0.0, 0.0, 0.0);
}
