#version 330 core

in vec3 vertex_position_worldspace;

uniform vec3 cameraPosition;

out vec4 fragmentColor;

void main()
{
    fragmentColor = vec4(vertex_position_worldspace.z - cameraPosition.z);
}