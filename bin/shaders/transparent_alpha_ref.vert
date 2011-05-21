uniform float CamFar;
uniform float Repeat;

varying vec3 Normal;
varying float Depth;

void main()
{
    vec4 vertex = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;

    Normal= normalize(gl_NormalMatrix * gl_Normal);
    Depth= (gl_ModelViewMatrix * gl_Vertex).z / CamFar;

    gl_Position= vertex;
    gl_TexCoord[0]= gl_MultiTexCoord0 * Repeat;
}