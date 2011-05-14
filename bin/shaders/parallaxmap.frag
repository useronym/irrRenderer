varying vec3 Normal;
varying vec3 Tangent;
varying vec3 Binormal;
varying vec3 ViewVect;

varying float Depth;

uniform sampler2D Tex0;
uniform sampler2D Tex1; //oh yes, you're right!

uniform float Param1; //scale

void main()
{
    vec4 normalFromTex= texture2D(Tex1, gl_TexCoord[0].xy);

    float bias = Param1 * 0.5;
    float height= Param1 * normalFromTex.a - bias;
    vec2 offsetCoord = gl_TexCoord[0].xy + normalize(ViewVect).xy * height;

    normalFromTex= texture2D(Tex1, offsetCoord);
    normalFromTex= normalFromTex * 2.0 - 1.0;
    mat3 tangentToView= mat3(Tangent, Binormal, Normal);
    normalFromTex.xyz= normalize(tangentToView * normalFromTex.xyz);
    normalFromTex.xyz*= 0.5;
    normalFromTex.xyz+= 0.5;

    gl_FragData[0] = texture2D(Tex0, offsetCoord);
    gl_FragData[1] = vec4(normalFromTex.xyz, 0.0);
    gl_FragData[2] = vec4(Depth, 0.0, 0.0, 0.0);
}
