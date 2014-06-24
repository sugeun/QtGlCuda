varying vec4 vVaryingColor;
varying vec4 vTexCord;

uniform sampler2D sphereTexmap;

void main()
{
    //gl_FragColor = vVaryingColor;
    gl_FragColor = texture2D(sphereTexmap, vTexCord.st);
}
