#version 330 core
layout (location = 0) in vec2 vertex_pos;
layout (location = 1) in vec3 vertex_normal;
layout (location = 2) in vec2 vertex_uv;

out vec2 vertexUV;

void main()
{
    gl_Position = vec4(vertex_pos.x, vertex_pos.y, 0.0, 1.0); 
    vertexUV = vertex_uv;
}  