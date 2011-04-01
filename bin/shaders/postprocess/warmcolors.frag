uniform sampler2D Render;

void main()
{
    vec4 color= texture2D(Render, gl_TexCoord[0].xy);
    color.r*= 1.20;
    color.g*= 1.15;
    color.b*= 0.90;

    gl_FragColor= color;
}

