uniform float CamFar;
uniform float Repeat;
uniform float Lighting;

varying vec3 Normal;
varying vec3 Tangent;
varying vec3 Binormal;
varying vec3 ViewVect;

varying float Depth;

void main()
{
    vec4 vertex = gl_ModelViewMatrix * gl_Vertex;
    ViewVect= normalize(gl_ModelViewMatrixInverse * gl_Vertex).xyz;
    vertex= gl_ProjectionMatrix * vertex;

    Depth= (gl_ModelViewMatrix * gl_Vertex).z / CamFar;

    Normal= normalize((gl_NormalMatrix * gl_Normal)).xyz;
    Tangent= normalize((gl_NormalMatrix * gl_MultiTexCoord1.xyz)).xyz;
    Binormal= normalize((gl_NormalMatrix * gl_MultiTexCoord2.xyz)).xyz;

    gl_Position= vertex;
    gl_TexCoord[0]= gl_MultiTexCoord0 * Repeat;
}
