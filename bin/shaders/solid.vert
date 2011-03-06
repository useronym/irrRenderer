uniform float CamFar;
uniform float Repeat;
uniform float Lighting;

varying vec2 Normal;
varying vec2 Depth; // + matID

void main()
{
   Normal= (gl_NormalMatrix * gl_Normal).xy;
   Normal*= 0.5;
   Normal+= 0.5;
   vec4 vertex = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
   Depth.x= vertex.z/CamFar;
   Depth.y= Lighting;

   gl_Position= vertex;
   gl_TexCoord[0]= gl_MultiTexCoord0 * Repeat;
}
