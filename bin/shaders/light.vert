uniform vec3 VertexLightPos;
uniform float VertexLightRange;
uniform vec3 VertexFarLeftUp;

varying vec4 ScreenPos;
varying vec3 LightPos;
varying float LightRange;
varying vec3 FarLeftUp;
varying float FarRightUpX;
varying float FarLeftDownY;

void main()
{
    ScreenPos= ftransform();
    gl_Position= ScreenPos;
    gl_TexCoord[0]= gl_MultiTexCoord0;
    LightPos= VertexLightPos;
    LightRange= VertexLightRange;
    FarLeftUp= VertexFarLeftUp;
    FarLeftDownY= FarLeftUp.y * -1;
    FarRightUpX= FarLeftUp.x * -1;
}
