varying vec2 Normal;
varying float Depth;

uniform sampler2D Tex0;

void main()
{
   gl_FragData[0] = texture2D(Tex0, gl_TexCoord[0].xy);
   gl_FragData[1] = vec4(Normal, Depth, 0.0);
}
