uniform sampler2D ColorTex;     //guess what :P
uniform sampler2D NormalTex;    //view space normal, view space depth

varying vec3 FarLeftUp;
varying float FarRightUpX;
varying float FarLeftDownY;

void main()
{
    //reconstruct normal and get depth
        vec4 vNormal= texture2D(NormalTex, gl_TexCoord[0].xy);
        float vDepth= vNormal.w;
        vNormal*= 2.0;
        vNormal-= 1.0;
        vNormal= normalize(vNormal);

    //reconstruct view pixel position
        vec3 vProjPos= vec3(mix(FarLeftUp.x, FarRightUpX, gl_TexCoord[0].x),
                            mix(FarLeftUp.y, FarLeftDownY, 1 - gl_TexCoord[0].y),
                            FarLeftUp.z);
        vec4 vPixelPos= vec4(vProjPos * vDepth, 0.0);

    float light= max(dot(vNormal, normalize(gl_LightSource[0].position - vPixelPos)), 0.0);

    vec4 color= vec4(texture2D(ColorTex, gl_TexCoord[0].xy) * light);
    gl_FragColor= color;
}
