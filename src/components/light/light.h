#pragma once

#include <common/model.h>
#include <glm/glm.hpp>
#include <vector>

using namespace glm;
using namespace std;

class Light
{
public:
    vec3 position;
    vec4 color;
    float intensity;
    float radius;

    static constexpr float nearPlane = 0.5f;
    static constexpr float farPlane = 30.0f;
    static constexpr float light_displacement = 1.0f;
    vec3 direction;

    mat4 projectionMatrix;
    mat4 viewMatrix;
    mat4 shadowTransforms[6];

    Light(vec3 position, vec4 color, float intensity, float radius);
    void upload_to_shaders(GLuint shaderProgram);
    void upload_depth_shader(GLuint shaderProgram);
    void update_shadow_transforms();

    void render(GLuint modelMatrixLocation, GLuint materialLocation[4]);
};