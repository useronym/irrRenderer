uniform sampler2D ColorTex;     //guess what :P
uniform sampler2D NormalTex;
uniform vec3 Color;

void main()
{
    vec4 texColor= texture2D(ColorTex, gl_TexCoord[0].xy);
    vec3 vNormal= texture2D(NormalTex, gl_TexCoord[0].xy).xyz;
    if(vNormal.x + vNormal.y + vNormal.z == 0.0) gl_FragColor= texColor;
    else gl_FragColor= texColor * vec4(Color, 0.0);
}
