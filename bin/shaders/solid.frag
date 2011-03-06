varying vec2 Normal;
varying vec2 Depth;

uniform sampler2D Tex0;

void main()
{
   gl_FragData[0] = texture2D(Tex0, gl_TexCoord[0].xy);
   gl_FragData[1] = vec4(Normal, Depth);
}
