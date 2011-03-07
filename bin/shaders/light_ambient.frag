uniform sampler2D ColorTex;     //guess what :P
uniform vec3 Color;

void main()
{
    gl_FragColor= texture2D(ColorTex, gl_TexCoord[0].xy) * vec4(Color, 0.0);
}
