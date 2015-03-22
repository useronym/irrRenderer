varying vec3 Normal;
varying vec3 Tangent;
varying vec3 Binormal;
varying vec3 ViewVect;

varying float Depth;

uniform sampler2D Tex0;
uniform sampler2D Tex1; //normal map

uniform float Param1; //bumpiness

void main()
{
    mat3 tangentToView= mat3(normalize(Tangent), normalize(Binormal), normalize(Normal));

    vec4 normalFromTex= texture2D(Tex1, gl_TexCoord[0].xy);

    float bumpiness= Param1;
    //float bias = bumpiness * 0.5;
    float heightFromTexture= normalFromTex.a * 2.0 - 1.0;
    float height= bumpiness * heightFromTexture;// - bias;
    vec2 offsetCoord= gl_TexCoord[0].xy + normalize(ViewVect).xy * height;
    normalFromTex= texture2D(Tex1, offsetCoord);

    normalFromTex= normalFromTex * 2.0 - 1.0;
    normalFromTex.xyz= normalize(tangentToView * normalFromTex.xyz);
    normalFromTex.xyz*= 0.5;
    normalFromTex.xyz+= 0.5;

    gl_FragData[0] = texture2D(Tex0, offsetCoord);
    gl_FragData[1] = vec4(normalFromTex.xyz, 0.0);
    gl_FragData[2] = vec4(Depth, 0.0, 0.0, 0.0);
}
