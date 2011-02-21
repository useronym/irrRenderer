uniform float VertexCamFar;
uniform vec3 VertexFarLeftUp;

varying vec3 FarLeftUp;
varying float FarRightUpX;
varying float FarLeftDownY;

void main()
{
    gl_Position= gl_Vertex;
    gl_TexCoord[0]= gl_MultiTexCoord0;
    gl_TexCoord[0].x= 1 - gl_MultiTexCoord0.x;
    FarLeftUp= VertexFarLeftUp;
    FarLeftDownY= FarLeftUp.y * -1;
    FarRightUpX= FarLeftUp.x * -1;
}
