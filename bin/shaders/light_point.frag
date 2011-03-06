uniform sampler2D ColorTex;     //guess what :P
uniform sampler2D NormalTex;    //view space normal.xy, view space depth, matID
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
    vec4 projCoord= ScreenPos / ScreenPos.w;
    projCoord+= vec4(1.0);
    projCoord*= 0.5;
    projCoord.xy= clamp(projCoord.xy, 0.001, 0.999);

    //get depth
    vec4 vNormal= texture2D(NormalTex, projCoord.xy);
    float vDepth= vNormal.z;

    if(vDepth * CamFar > ScreenPos.z) discard;

    vec4 texColor= texture2D(ColorTex, projCoord.xy);

    //reconstruct view pixel position
    vec3 vProjPos= vec3(mix(FarLeftUp.x, FarRightUpX, projCoord.x),
                        mix(FarLeftUp.y, FarLeftDownY, 1.0 - projCoord.y),
                        FarLeftUp.z);
    vec4 vPixelPos= vec4(vProjPos * vDepth, 0.0);

    vec4 lightPos= vec4(Position, 0.0);
    float dist= length(lightPos - vPixelPos);

    if(dist < Radius)
    {
        //reconstruct normal
        vNormal.xy*= 2.0;
        vNormal.xy-= 1.0;
        vNormal.z= -sqrt( -(vNormal.x*vNormal.x) - (vNormal.y*vNormal.y) + 1.0 );

        //calculate the light
        float att= max(1 - (dist/Radius), 0.0);
        float light= max(dot(normalize(lightPos - vPixelPos), vNormal), 0.0) * att;

        vec4 lightColor= vec4(Color, 0.0);
        texColor*= light * lightColor;
    }
    else discard;

    gl_FragColor= texColor;
}
