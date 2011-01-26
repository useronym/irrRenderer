uniform sampler2D ColorTex;     //guess what :P
uniform sampler2D NormalTex;    //view space normal, view space depth

uniform float CamFar;

void main()
{
    //reconstruct normal and get depth
        vec4 vNormal= texture2D(NormalTex, gl_TexCoord[0].xy);
        float vDepth= vNormal.w;
        vNormal*= 2;
        vNormal-= 1;
        vNormal= normalize(vNormal);

    //reconstruct view pixel position
        //get the ray to the far plane
        vec3 vRay= vec3(mix(-0.5, 0.5, gl_TexCoord[0].x),
                        mix(0.5, -0.5, gl_TexCoord[0].y),
                        vDepth);
        vec4 vPixelPos= vec4(vRay * CamFar, 1.0);

    //calculate view vector
    vec4 vVect= normalize(vPixelPos);

    float light= max(dot(vNormal, vVect), 0.0);

    vec4 color= texture2D(ColorTex, gl_TexCoord[0].xy) * light;
    gl_FragColor = color;
}
