uniform float CamFar;
uniform float Repeat;

varying vec3 Normal;
varying float Depth;

void main()
{
   Normal= normalize(gl_Normal * gl_NormalMatrix);
   Normal*= 0.5;
   Normal+= 0.5;
   vec4 vertex = ftransform();
   Depth= vertex.z/CamFar;

   gl_Position= vertex;
   gl_TexCoord[0]= gl_MultiTexCoord0 * Repeat;
   gl_TexCoord[1]= gl_MultiTexCoord0;
}
