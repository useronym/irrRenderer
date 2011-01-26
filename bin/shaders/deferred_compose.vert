void main()
{
   vec4 vertex= gl_Vertex;
   gl_Position= vertex;
   gl_TexCoord[0]= gl_MultiTexCoord0;
   gl_TexCoord[0].x*= -1;
}
