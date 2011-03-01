uniform sampler2D ColorTex;     //guess what :P
uniform sampler2D NormalTex;    //view space normal.xy, view space depth
uniform vec3 Position;
uniform float Radius;


varying vec4 ScreenPos;
varying vec3 FarLeftUp;
varying float FarRightUpX;
varying float FarLeftDownY;


void main()
{
    //need some cool tex coords in here!
    vec4 projCoord= ScreenPos / ScreenPos.w;
    projCoord+= vec4(1.0);
    projCoord*= 0.5;
    projCoord.xy= clamp(projCoord.xy, 0.001, 0.999);

    //get depth
    vec4 vNormal= texture2D(NormalTex, projCoord.xy);
    vec4 col= vNormal;
    float vDepth= vNormal.z;

    //if(vDepth < ScreenPos.z)
    {
        //reconstruct normal
        vNormal.xy*= 2.0;
        vNormal.xy-= 1.0;
        vNormal.z= -sqrt(-(vNormal.x*vNormal.x) - (vNormal.y*vNormal.y) + 1.0);

        //reconstruct view pixel position
        vec3 vProjPos= vec3(mix(FarLeftUp.x, FarRightUpX, projCoord.x),
                            mix(FarLeftUp.y, FarLeftDownY, 1 - projCoord.y),
                            FarLeftUp.z);
        vec4 vPixelPos= vec4(vProjPos * vDepth, 0.0);

        //calculate the light
        //float l= length(gl_LightSource[0].position - vPixelPos);
        //float att= max(1.0-(l/200.0), 0.0);
        //float light= max(dot(normalize(gl_LightSource[0].position - vPixelPos), vNormal), 0.0) * att;

        //vec4 color= vec4(texture2D(ColorTex, gl_TexCoord[0].xy) * light);
        gl_FragColor= vec4(1.0, vDepth, 0.0, 1.0);//color;
    }
    //else discard;
}
