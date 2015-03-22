varying vec2 Normal;
varying float Depth;

uniform sampler2D Tex0;
uniform sampler2D Tex1;

uniform float Param1;

void main()
{
    vec4 splat= texture2D(Tex0, gl_TexCoord[0].xy * Param1);
    vec4 detail= texture2D(Tex1, gl_TexCoord[1].xy);

    gl_FragData[0] = splat * detail;
    gl_FragData[1] = vec4(Normal, 0.0, 0.0);
    gl_FragData[2] = vec4(Depth, 0.0, 0.0, 0.0);
}
