uniform sampler2D Render;
uniform sampler2D MRT0;
uniform sampler2D MRT1;
uniform sampler2D MRT2;

void main()
{
    vec4 color= texture2D(Render, gl_TexCoord[0].xy);
    color.g= color.r + color.g + color.b;
    color.r= 0;
    color.b= 0;
    gl_FragColor= color;
}
