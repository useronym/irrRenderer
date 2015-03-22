void main()
{
    gl_Position= gl_Vertex;
    gl_TexCoord[0]= gl_MultiTexCoord0;
    gl_TexCoord[0].y= 1.0 - gl_TexCoord[0].y; //irlicht devs ftl
}

