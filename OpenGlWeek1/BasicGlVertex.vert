attribute vec4 vertices;
varying vec2 coords;
uniform mat4 matrix;

void main()
{
    gl_Position = matrix * vertices;
    coords = vertices.xy;
}
