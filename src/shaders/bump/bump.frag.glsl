#version 330 core

in vec3 vertex_position_worldspace;
in vec3 vertex_normal;

uniform vec3 cameraPosition;

out vec3 normal;

void main()
{
    // flip normal. In secondary room we are looking torwards +z but in main room torwards -z
    normal = normalize(vec3(-vertex_normal.x, vertex_normal.y, -vertex_normal.z)) * 0.5 + 0.5;
    gl_FragDepth = gl_FragCoord.z / gl_FragCoord.w - cameraPosition.z;
}
