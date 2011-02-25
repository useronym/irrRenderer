uniform sampler2D ColorTex;     //guess what :P
uniform sampler2D NormalTex;    //view space normal.xy, view space depth

varying vec4 ScreenPos;
varying vec3 LightPos;
varying float LightRange;
varying vec3 FarLeftUp;
varying float FarRightUpX;
varying float FarLeftDownY;

void main()
{
    //get depth
    vec4 vNormal= texture2D(NormalTex, gl_TexCoord[0].xy);
    float vDepth= vNormal.z;

    if(vDepth < ScreenPos.z)
    {
        //reconstruct normal
        vNormal.xy*= 2.0;
        vNormal.xy-= 1.0;
        vNormal.z= -sqrt(-(vNormal.x*vNormal.x) - (vNormal.y*vNormal.y) + 1.0);

        //reconstruct view pixel position
        vec3 vProjPos= vec3(mix(FarLeftUp.x, FarRightUpX, gl_TexCoord[0].x),
                            mix(FarLeftUp.y, FarLeftDownY, 1 - gl_TexCoord[0].y),
                            FarLeftUp.z);
        vec4 vPixelPos= vec4(vProjPos * vDepth, 0.0);
    }
    else discard;

    float l= length(gl_LightSource[0].position - vPixelPos);
    float att= max(1.0-(l/200.0), 0.0);
    float light= max(dot(normalize(gl_LightSource[0].position - vPixelPos), vNormal), 0.0) * att;

    vec4 color= vec4(texture2D(ColorTex, gl_TexCoord[0].xy) * light);
    gl_FragColor= color;
}
