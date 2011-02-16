uniform float VertexCamFar;
uniform vec3 VertexFarLeftUp;
uniform mat4 VMProjInv;

varying float CamFar;
varying vec3 FarLeftUp;
varying vec3 FarLeftDown;
varying vec3 FarRightUp;
varying mat4 MProjInv;


void main()
{
    vec4 vertex= gl_Vertex;
    gl_Position= vertex;
    gl_TexCoord[0]= gl_MultiTexCoord0;
    gl_TexCoord[0].x= 1 - gl_MultiTexCoord0.x;
    CamFar= VertexCamFar;
    FarLeftUp= FarLeftDown= FarRightUp= VertexFarLeftUp;
    FarLeftDown.y*= -1;
    FarRightUp.x*= -1;
    MProjInv= VMProjInv;
}
