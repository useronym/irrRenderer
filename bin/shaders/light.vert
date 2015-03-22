uniform mat4 WorldViewProjMat;
uniform mat4 WorldViewMat;
uniform vec3 VertexFarLeftUp;

varying vec4 ScreenPos;
varying vec3 FarLeftUp;
varying float FarRightUpX;
varying float FarLeftDownY;

void main()
{
    ScreenPos= WorldViewProjMat * gl_Vertex;
    gl_Position= ScreenPos;
    FarLeftUp= VertexFarLeftUp;
    FarLeftDownY= -FarLeftUp.y;
    FarRightUpX= -FarLeftUp.x;
}
