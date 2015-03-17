uniform sampler2D ColorTex;     //color
uniform sampler2D NormalTex;    //view space normal.xy
uniform sampler2D DepthTex;    //view space depth
uniform float CamFar;

uniform vec3 Position;
uniform float Radius;
uniform vec3 Color;
uniform vec3 Direction;
uniform float CosCutoff;
uniform float Falloff;


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
    float vDepth= texture2D(DepthTex, projCoord.xy).r;

    if(vDepth * CamFar > ScreenPos.z) discard;


    //reconstruct view pixel position
    vec3 vProjPos= vec3(mix(FarLeftUp.x, FarRightUpX, projCoord.x),
                        mix(FarLeftUp.y, FarLeftDownY, 1.0 - projCoord.y),
                        FarLeftUp.z);
    vec3 vPixelPos= vProjPos * vDepth;

    vec3 vLightPos= Position;
    float dist= length(vLightPos - vPixelPos);

    if(dist < Radius)
    {
        //reconstruct normal
        vec3 vNormal= texture2D(NormalTex, projCoord.xy).xyz;
        vNormal*= 2.0;
        vNormal-= 1.0;
        //vNormal.z= -sqrt(1.0 - (vNormal.x*vNormal.x) - (vNormal.y*vNormal.y));
        //vNormal= normalize(vNormal);

        //calculate the light
        vec3 lightDir= normalize(vLightPos - vPixelPos);
        vec3 spotDir= normalize(Direction);
        float cosAngle = dot(spotDir, -lightDir);

        if(cosAngle > CosCutoff)
        {
            //float attLinear = 1.0 / Radius;
            //float attQuadratic= attLinear / Radius;

            //make the edges "soft"
            float softEdge= (cosAngle - CosCutoff) / (1.0 - CosCutoff);//pow(cosAngle, Falloff);
            softEdge= pow(softEdge, Falloff);
            float att= softEdge * max(-log(dist/Radius), 0.0);
            /*float att= cosAngle / (dist * attLinear) + (dist * dist * attQuadratic);
            att-= 2.0;*/

            float light= max(dot(lightDir, vNormal), 0.0) * att;

            vec3 lightColor= Color;
            gl_FragColor= light * vec4(lightColor, 0.0) * texture2D(ColorTex, projCoord.xy);
        }
        else discard;
    }
    else discard;
}
