uniform float CamFar;
uniform float Repeat;
uniform float Lighting;

varying vec2 Normal;
varying float Depth;

void main()
{
    vec4 vertex = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
    Depth= vertex.z/CamFar;

    if(Lighting == 0.0)
    {
        Normal= vec2(0.0,0.0);
        Depth= 1.0; //automatically Z-reject these areas
    }
    else
    {
        Normal= normalize((gl_NormalMatrix * gl_Normal)).xy;
        Normal*= 0.5;
        Normal+= 0.5;
    }

    gl_Position= vertex;
    gl_TexCoord[0]= gl_MultiTexCoord0 * Repeat;
}
