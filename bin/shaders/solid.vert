uniform float CamFar;
uniform float Repeat;

varying vec3 Normal;
varying float Depth;

void main()
{
   Normal= gl_NormalMatrix * gl_Normal;
   Normal*= 0.5;
   Normal+= 0.5;
   vec4 vertex = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
   Depth= vertex.z/CamFar;

   gl_Position= vertex;
   gl_TexCoord[0]= gl_MultiTexCoord0 * Repeat;
}
