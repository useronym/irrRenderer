uniform sampler2D ColorTex;     //guess what :P
uniform sampler2D NormalTex;    //view space normal, view space depth

varying float CamFar;
varying vec3 FarLeftUp;
varying vec3 FarLeftDown;
varying vec3 FarRightUp;
varying mat4 MProjInv;

void main()
{
    //reconstruct normal and get depth
        vec4 vNormal= texture2D(NormalTex, gl_TexCoord[0].xy);
        float vDepth= vNormal.w;
        vNormal*= 2.0;
        vNormal-= 1.0;
        vNormal= normalize(vNormal);

    //reconstruct view pixel position

        vec3 vProjPos= vec3(mix(FarLeftUp.x, FarRightUp.x, gl_TexCoord[0].x),
                            mix(FarLeftUp.y, FarLeftDown.y, 1 - gl_TexCoord[0].y),
                            CamFar);
        vec4 vPixelPos= vec4(vProjPos, 0.0);
        vPixelPos*= vDepth;
        vPixelPos.z= vDepth * CamFar;

    float light= max(dot(vNormal, normalize(gl_LightSource[0].position - vPixelPos)), 0.0);

    vec4 color= normalize(vPixelPos);//vec4(texture2D(ColorTex, gl_TexCoord[0].xy) * light);
    gl_FragColor= color;
}
