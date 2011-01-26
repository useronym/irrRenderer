varying vec3 Normal;
varying float Depth;

uniform sampler2D Tex0;
uniform sampler2D Tex1;
uniform sampler2D Tex2;
uniform sampler2D Tex3;

void main()
{
   vec4 texel0= texture2D(Tex0, gl_TexCoord[0].xy);
   vec4 texel1= texture2D(Tex1, gl_TexCoord[0].xy);
   vec4 detail= (texture2D(Tex2, gl_TexCoord[0].xy) - 0.5f);
   vec4 splatmap= texture2D(Tex3, gl_TexCoord[1].xy);
   float splatindex= splatmap.r + splatmap.g;
   splatindex= clamp(0.0, 1.0, splatindex);

   vec4 color= detail;
   color+= mix(texel1, texel0, splatindex);

   gl_FragData[0] = color;
   gl_FragData[1] = vec4(Normal, Depth);
}
