uniform sampler2D Render;

void main()
{
    vec4 sum = vec4(0);
    vec2 texcoord = vec2(gl_TexCoord[0]);
    int j;
    int i;

    for(i= -3; i < 3; i++)
    {
        for (j= -3; j < 3; j++)
        {
            sum+= texture2D(Render, texcoord + vec2(j, i)*0.004) * 0.03;
        }
    }

    gl_FragColor= sum + texture2D(Render, texcoord);
}
