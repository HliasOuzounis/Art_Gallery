#version 330 core

in vec3 vertex_position_worldspace;
in vec3 vertex_normal;

uniform vec3 cameraPosition;

out vec3 normal;

void main()
{
    normal = vertex_normal * 0.5 + 0.5;
    gl_FragDepth = vertex_position_worldspace.z;
}
