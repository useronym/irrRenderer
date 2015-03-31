uniform sampler2D ColorTex;     //guess what :P
uniform sampler2D NormalTex;    //view space normal.xy
uniform sampler2D DepthTex;    //view space depth
uniform float CamFar;
uniform vec3 Position;
uniform float Radius;
uniform vec3 Color;


varying vec4 ScreenPos;
varying vec3 FarLeftUp;
varying float FarRightUpX;
varying float FarLeftDownY;


void main()
{
    //need some cool tex coords in here!
    vec4 projCoord = ScreenPos / ScreenPos.w;
    projCoord += vec4(1.0);
    projCoord *= 0.5;
    projCoord.xy = clamp(projCoord.xy, 0.001, 0.999);

    float vDepth = texture2D(DepthTex, projCoord.xy).r;

    if(vDepth * CamFar > ScreenPos.z) discard; //Z-reject
    

    //reconstruct view pixel position
    vec3 vProjPos = vec3(mix(FarLeftUp.x, FarRightUpX, projCoord.x),
                        mix(FarLeftDownY, FarLeftUp.y, projCoord.y),
                        FarLeftUp.z);
    vec3 vPixelPos = vec3(vProjPos * vDepth);

    vec3 vLightPos = vec3(Position);
    float dist = length(vLightPos - vPixelPos);

    if(dist < Radius)
    {
        vec3 vNormal = texture2D(NormalTex, projCoord.xy).rgb;
        vNormal *= 2.0;
        vNormal -= 1.0;

        float att = max(-log(dist/Radius), 0.0);
        float light = max(dot(normalize(vLightPos - vPixelPos), vNormal), 0.0) * att;

        vec3 lightColor = Color;
        gl_FragColor = light * vec4(lightColor, 0.0) * texture2D(ColorTex, projCoord.xy);
    }
    else discard;
}
