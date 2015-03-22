uniform mat4 WorldViewProjMat;
uniform mat4 WorldViewMat;
uniform float CamFar;

varying vec3 Normal;
varying float Depth;

void main()
{
    vec4 vertex = WorldViewProjMat * gl_Vertex;

    /*if(Lighting == 0.0)
    {
        Normal= vec3(0.0);
        Depth= 1.0; //automatically Z-reject these areas
    }
    else
    {*/
        Normal= normalize((gl_NormalMatrix * gl_Normal)).xyz;

        vec4 vVertex= WorldViewMat * gl_Vertex;
        Depth= vVertex.z/CamFar;
    //}

    gl_Position= vertex;
    gl_TexCoord[0]= gl_MultiTexCoord0;
}
