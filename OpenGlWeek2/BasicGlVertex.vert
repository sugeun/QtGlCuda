attribute vec4 vertices;
//attribute vec3 vNormal;
attribute vec4 texCoord;

varying vec4 vVaryingColor;
varying vec4 vTexCord;

//uniform vec3 vLightPosition;
uniform mat4 mvpMatrix;
//uniform mat4 mvMatrix;
//uniform mat3 normalMatrix;

void main()
{
    vec4 baseColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);

    float theta, phi;

    theta = vertices.x * 2.0f * 3.14f;
    phi = (1.0f - vertices.y) * 3.14f;

    vec4 sphereVertex = vec4(0.0f, 0.0f, 0.0f, 1.0f);

    sphereVertex.x = sin(theta) * sin(phi); //y
    sphereVertex.y = cos(phi); //z
    sphereVertex.z = cos(theta) * sin(phi); //x

    vVaryingColor.rgb = vertices.rgb;
    vVaryingColor.a = 1.0;
    gl_Position = mvpMatrix * sphereVertex;
    //gl_Position = mvpMatrix * vertices;
    //vTexCord = vertices;
    vTexCord = texCoord;
}
