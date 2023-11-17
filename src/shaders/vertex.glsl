#version 330 core

// input vertex and UV coordinates, different for all executions of this shader
layout(location = 0) in vec3 vertexPosition_modelspace;

// Output data ; will be interpolated for each fragment.
out vec4 vertexPosition_worldspace;

// model view projection matrix 
uniform mat4 MVP;

void main()
{
    // assign vertex position
    vertexPosition_worldspace = MVP * vec4(vertexPosition_modelspace, 1.0);
    gl_Position = vertexPosition_worldspace;
}
