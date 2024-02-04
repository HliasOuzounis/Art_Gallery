#version 330 core

in vec3 vertex_position_worldspace;
in vec3 vertex_normal;

out vec3 normal;

void main()
{
    normal = vertex_normal * 0.5 + 0.5;
}
