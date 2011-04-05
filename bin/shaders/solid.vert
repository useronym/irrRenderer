uniform float CamFar;
uniform float Repeat;
uniform float Lighting;

varying vec3 Normal;
varying float Depth;

void main()
{
    vec4 vertex = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;

    if(Lighting == 0.0)
    {
        Normal= vec3(0.0,0.0,0.0);
        Depth= 1.0; //automatically Z-reject these areas
    }
    else
    {
        Normal= normalize((gl_NormalMatrix * gl_Normal)).xyz;

        Depth= vertex.z/CamFar;
    }

    gl_Position= vertex;
    gl_TexCoord[0]= gl_MultiTexCoord0 * Repeat;
}
