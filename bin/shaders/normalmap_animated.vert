#version 130

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
    vec3 Tangent= -vec3(abs(gl_Normal.y) + abs(gl_Normal.z), abs(gl_Normal.x), 0);
    vec3 Binormal= cross(Tangent, gl_Normal);
    Tangent= normalize(gl_NormalMatrix * cross(Binormal, gl_Normal));
    Binormal= normalize(gl_NormalMatrix * Binormal);


    gl_Position= vertex;
    gl_TexCoord[0]= gl_MultiTexCoord0;
}

