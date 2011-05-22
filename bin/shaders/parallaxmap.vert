uniform float CamFar;
uniform float Lighting;

varying vec3 Normal;
varying vec3 Tangent;
varying vec3 Binormal;
varying vec3 ViewVect;

varying float Depth;

void main()
{
    vec4 vertex = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
    ViewVect= -normalize(gl_ModelViewMatrix * gl_Vertex).xyz;

    Depth= (gl_ModelViewMatrix * gl_Vertex).z / CamFar;

    Normal= normalize((gl_NormalMatrix * gl_Normal)).xyz;
    Tangent= normalize((gl_NormalMatrix * gl_MultiTexCoord1.xyz)).xyz;
    Binormal= normalize((gl_NormalMatrix * gl_MultiTexCoord2.xyz)).xyz;

    mat3 tangentToView= mat3(Tangent, Binormal, Normal);
    ViewVect= normalize(inverse(tangentToView) * ViewVect);
    ViewVect.y= -ViewVect.y;

    gl_Position= vertex;
    gl_TexCoord[0]= gl_MultiTexCoord0;
}
