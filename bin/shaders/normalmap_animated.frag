varying vec3 Normal;
varying vec3 ViewVect;
varying float Depth;

uniform sampler2D Tex0;
uniform sampler2D Tex1;

// http://www.thetenthplanet.de/archives/1180
mat3 cotangent_frame(vec3 N, vec3 p, vec2 uv)
{
    // get edge vectors of the pixel triangle
    vec3 dp1 = dFdx( p );
    vec3 dp2 = dFdy( p );
    vec2 duv1 = dFdx( uv );
    vec2 duv2 = dFdy( uv );

    // solve the linear system
    vec3 dp2perp = cross( dp2, N );
    vec3 dp1perp = cross( N, dp1 );
    vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
    vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;

    // construct a scale-invariant frame
    float invmax = inversesqrt( max( dot(T,T), dot(B,B) ) );
    return mat3( T * invmax, B * invmax, N );
}

vec3 perturb_normal(vec3 N, vec3 V, vec2 texcoord)
{
    // assume N, the interpolated vertex normal and
    // V, the view vector (vertex to eye)
    vec3 map = texture2D(Tex1, texcoord).xyz;
    map = map * 255.0/127.0 - 128.0/127.0;
    mat3 TBN = cotangent_frame(N, -V, texcoord);
    return normalize(TBN * map);
}

void main()
{
    /*vec3 normalFromTex = ((texture2D(Tex1, gl_TexCoord[0].xy) * 2.0) - 1.0).xyz;
    mat3 tangentToView = mat3(Tangent, Binormal, Normal);
    normalFromTex = normalize(tangentToView * normalFromTex);*/
    vec3 vertexN = normalize( Normal );
    vec3 viewV = normalize( ViewVect );
    vec3 normalFromTex = perturb_normal( vertexN, viewV, gl_TexCoord[0].xy);

    normalFromTex *= 0.5;
    normalFromTex += 0.5;

    gl_FragData[0] = texture2D(Tex0, gl_TexCoord[0].xy);
    gl_FragData[1] = vec4(normalFromTex, 0.0);
    gl_FragData[2] = vec4(Depth, 0.0, 0.0, 0.0);
}
