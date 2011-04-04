uniform float CamFar;
uniform float Repeat;
uniform float Lighting;

varying vec3 Normal;
varying vec3 Tangent;
varying vec3 Binormal;

varying float Depth;

void main()
{
    vec4 vertex = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;

    Normal= normalize((gl_NormalMatrix * gl_Normal)).xyz;

    Depth= vertex.z/CamFar;

    //calculate the tangent
    /*vec3 c1 = cross(gl_Normal, vec3(0.0, 0.0, 1.0));
    vec3 c2 = cross(gl_Normal, vec3(0.0, 1.0, 0.0));

    if(length(c1)>length(c2))
    {
        Tangent = c1;
    }
    else
    {
        Tangent = c2;
    }
    Tangent = normalize(Tangent);*/
    Tangent= -vec3(abs(gl_Normal.y) + abs(gl_Normal.z), abs(gl_Normal.x), 0);

    //and binormal
    Binormal = cross(Tangent, Normal);
    Binormal = normalize(Binormal);

    Tangent=gl_NormalMatrix*cross(Binormal,gl_Normal);
    Binormal=gl_NormalMatrix*Binormal;

    gl_Position= vertex;
    gl_TexCoord[0]= gl_MultiTexCoord0 * Repeat;
}

