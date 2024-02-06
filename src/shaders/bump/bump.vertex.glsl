#version 330 core

// input vertex and UV coordinates, different for all executions of this shader
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal_modelspace;

out vec3 vertex_position_worldspace;
out vec3 vertex_normal;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main()
{
    gl_Position = P * V * M * vec4(vertexPosition_modelspace, 1.0);

    vertex_position_worldspace = vec3(M * vec4(vertexPosition_modelspace, 1.0));

    vertex_normal = transpose(inverse(mat3(M))) * vertexNormal_modelspace;
}