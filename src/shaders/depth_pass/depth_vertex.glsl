#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;


uniform mat4 M;

void main()
{
    // Model space -> World space
    gl_Position =  M * vec4(vertexPosition_modelspace, 1);
}