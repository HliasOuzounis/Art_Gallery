#version 330 core

// input vertex and UV coordinates, different for all executions of this shader
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal_modelspace;
layout(location = 2) in vec2 vertexUV;

// model view projection matrix 
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

//lighting
struct Light {
    vec3 lightPosition;
    vec4 La;
    vec4 Ld;
    vec4 Ls;
    mat4 lightVP;
    float lightIntensity;
};
uniform Light light;

// Output data ; will be interpolated for each fragment.
out vec4 vertex_position_cameraspace;
out vec4 vertex_normal_cameraspace;
out vec4 light_position_cameraspace;
// out vec2 vertex_UV;
out vec4 vertex_position_lightspace;

void main()
{
    // assign vertex position
    gl_Position = P * V * M * vec4(vertexPosition_modelspace, 1.0);

    vertex_position_cameraspace = V * M * vec4(vertexPosition_modelspace, 1);
    vertex_normal_cameraspace = V * M * vec4(vertexNormal_modelspace, 0);
    light_position_cameraspace = V * vec4(light.lightPosition, 1);

    vertex_position_lightspace = light.lightVP * M * vec4(vertexPosition_modelspace, 1);
}
