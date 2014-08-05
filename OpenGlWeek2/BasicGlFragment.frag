uniform sampler2D sphereTexmap;

varying vec4 vVaryingColor;

varying vec4 vTexCord;
varying vec4 vVaryingNormal;

void main()
{
    vec4 color = texture2D(sphereTexmap, vTexCord.st);


    vec3 vLight = vec3(-300.0f, 150.0f, 100.0f);
    float diff = max(0.0f, dot( normalize(vVaryingNormal.xyz), normalize(vLight) ));
    vec4 diffuseColor = vec4( 1.0f, 1.0f, 1.0f, 1.0f);
    vec4 ambientColor = vec4(0.3f, 0.3f, 0.3f, 1.0f);
    diffuseColor *= diff ;
    gl_FragColor = diffuseColor;
    gl_FragColor += ambientColor;

    float normalLengh = length(vVaryingNormal.xy);

    float diffNormal =  dot( normalize(vVaryingNormal.xyz), vec3(0.0f,0.0f, 1.0f ));

    float amt = 1.0;
    amt = smoothstep(-0.2f,0.00f,diffNormal);

    gl_FragColor *= color;

    gl_FragColor = mix( gl_FragColor, vec4(0.0, 0.0, 0.0, 1.0),  amt );

}
