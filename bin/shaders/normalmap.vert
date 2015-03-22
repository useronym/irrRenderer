uniform mat4 WorldViewProjMat;
uniform mat4 WorldViewMat;
uniform float CamFar;
uniform float Lighting;

varying vec3 Normal;
varying vec3 Tangent;
varying vec3 Binormal;

varying float Depth;

void main()
{
    vec4 vertex = WorldViewProjMat * gl_Vertex;

    Depth= (WorldViewMat * gl_Vertex).z / CamFar;

    Normal= normalize((gl_NormalMatrix * gl_Normal)).xyz;
    Tangent= normalize((gl_NormalMatrix * gl_MultiTexCoord1.xyz)).xyz;
    Binormal= normalize((gl_NormalMatrix * gl_MultiTexCoord2.xyz)).xyz;

    gl_Position= vertex;
    gl_TexCoord[0]= gl_MultiTexCoord0;
}

