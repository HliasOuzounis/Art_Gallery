#version 330 core

in vec3 vertex_position_worldspace;
in vec3 vertex_normal;

uniform vec3 cameraPosition;

out vec4 fragmentColor;

void main()
{
    // flip normal. In secondary room we are looking torwards +z but in main room torwards -z
    fragmentColor = vec4(-vertex_normal.x, vertex_normal.y, -vertex_normal.z, 1.0) * 0.5 + 0.5;
}