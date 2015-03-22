#version 130

uniform mat4 WorldViewProjMat;
uniform mat4 WorldViewMat;
uniform float CamFar;
uniform float Lighting;

varying vec3 Normal;
varying vec3 ViewVect;
varying float Depth;

void main()
{
    vec4 vertex = WorldViewProjMat * gl_Vertex;

    Depth = ( WorldViewMat * gl_Vertex ).z / CamFar;

    ViewVect = normalize( vertex ).xyz * vec3(-1.0, 1.0, 1.0);
    Normal = normalize( gl_NormalMatrix * gl_Normal ).xyz;

    gl_Position = vertex;
    gl_TexCoord[0]= gl_MultiTexCoord0;
}

