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

    float theta, phi;

    theta = vertices.x * 2.0f * 3.14f;
    phi = vertices.y * 3.14f;

    vec4 sphereVertex = vec4(0.0f, 0.0f, 0.0f, 1.0f);

    sphereVertex.x = cos(theta) * sin(phi);
    sphereVertex.y = sin(theta) * sin(phi);
    sphereVertex.z = cos(phi);

    vVaryingColor.rgb = baseColor.rgb;
    vVaryingColor.a = 1.0;
    gl_Position = mvpMatrix * sphereVertex;
}
