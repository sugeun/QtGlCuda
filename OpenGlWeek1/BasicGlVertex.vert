attribute vec4 vertices;
attribute vec3 vNormal;

varying vec4 vVaryingColor;

uniform vec3 vLightPosition;
uniform mat4 mvpMatrix;
uniform mat4 mvMatrix;
uniform mat3 normalMatrix;

void main()
{
    vec4 baseColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);
    vec4 errorColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);

    vec3 vEyeNormal = normalMatrix * vNormal;

    vec4 vPos4 = mvMatrix * vertices;
    vec3 vPos3 = vPos4.xyz / vPos4.w;
    vec3 vLightDir = normalize( vLightPosition - vPos3 );

    float diffuse = max(0.0, dot(vEyeNormal, vLightDir));


    if( vPos4.z > 0.0 )
    {
      baseColor = errorColor;

    }
    //baseColor = errorColor;

    //vVaryingColor.xyz = diffuse * baseColor.xyz;
    vVaryingColor.xyz = baseColor.xyz;
    vVaryingColor.a = 1.0;
    gl_Position = mvpMatrix * vertices;
}
