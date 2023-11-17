#version 330 core

uniform vec3 color;

// output data
out vec4 fragmentColor;


void main()
{
    fragmentColor = vec4(color, 1.0);
}
