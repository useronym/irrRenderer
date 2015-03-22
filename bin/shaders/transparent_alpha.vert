uniform mat4 WorldViewProjMat;

void main()
{
    vec4 vertex = WorldViewProjMat * gl_Vertex;

    gl_Position= vertex;
    gl_TexCoord[0]= gl_MultiTexCoord0;
}
