#include <common/model.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <algorithm>

using namespace glm;
using namespace std;

#include <iostream>
#include "light.h"

Light::Light(vec3 position, vec4 color, float intensity, float radius)
    : position(position), color(color), intensity(intensity), radius(radius)
{
    direction = normalize(vec3(0, 0, 0) - position);
    projectionMatrix = perspective(radians(90.0f), 1.0f, nearPlane, farPlane);

    drawable = new Drawable("src/objects/sphere.obj");
    vector<vec3> normals;
    for (auto &n : drawable->normals)
        normals.push_back(-n);
    drawable = new Drawable(drawable->vertices, drawable->uvs, normals);

    update_shadow_transforms();
}

void Light::upload_to_shaders(GLuint shaderProgram)
{
    GLuint lightPosLocation = glGetUniformLocation(shaderProgram, "light.lightPos");
    if (lightPosLocation == -1)
    {
        fprintf(stderr, "Error: can't find light position uniforms\n");
        exit(EXIT_FAILURE);
    }
    glUniform3f(lightPosLocation, position.x, position.y, position.z);
    //*/
    GLuint LaLocation = glGetUniformLocation(shaderProgram, "light.La");
    if (LaLocation == -1)
    {
        fprintf(stderr, "Error: can't find light ambient uniforms\n");
        exit(EXIT_FAILURE);
    }
    glUniform4f(LaLocation, color.r * 0.1, color.g * 0.1, color.b * 0.1, 1.0f);

    GLuint LdLocation = glGetUniformLocation(shaderProgram, "light.Ld");
    if (LdLocation == -1)
    {
        fprintf(stderr, "Error: can't find light diffuse uniforms\n");
        exit(EXIT_FAILURE);
    }
    glUniform4f(LdLocation, color.r, color.g, color.b, 1.0f);

    GLuint LsLocation = glGetUniformLocation(shaderProgram, "light.Ls");
    if (LsLocation == -1)
    {
        fprintf(stderr, "Error: can't find light specular uniforms\n");
        exit(EXIT_FAILURE);
    }    glUniform4f(LsLocation, color.r, color.g, color.b, 1.0f);
    //*/
    
    GLuint farPlaneLocation = glGetUniformLocation(shaderProgram, "light.farPlane");
    if (farPlaneLocation == -1)
    {
        fprintf(stderr, "Error: can't find light far plane uniforms\n");
        exit(EXIT_FAILURE);
    }
    glUniform1f(farPlaneLocation, farPlane);
    /*/
    GLuint lightIntensityLocation = glGetUniformLocation(shaderProgram, "light.lightIntensity");
    if (lightIntensityLocation == -1)
    {
        fprintf(stderr, "Error: can't find light intensity uniforms\n");
        exit(EXIT_FAILURE);
    }
    glUniform1f(lightIntensityLocation, intensity);
    //*/
}

void Light::upload_depth_shader(GLuint shaderProgram)
{
    GLuint lightPosLocation = glGetUniformLocation(shaderProgram, "light.lightPos");
    if (lightPosLocation == -1)
    {
        fprintf(stderr, "Error: can't find light position uniforms in depth shader\n");
        exit(EXIT_FAILURE);
    }
    glUniform3f(lightPosLocation, position.x, position.y, position.z);

    GLuint farPlaneLocation = glGetUniformLocation(shaderProgram, "light.farPlane");
    if (farPlaneLocation == -1)
    {
        fprintf(stderr, "Error: can't find light far plane uniforms in depth shader\n");
        exit(EXIT_FAILURE);
    }
    glUniform1f(farPlaneLocation, farPlane);

    GLuint shadowTransformsLocation = glGetUniformLocation(shaderProgram, "light.shadowTransforms");
    if (shadowTransformsLocation == -1)
    {
        fprintf(stderr, "Error: can't find light shadow transforms uniforms in depth shader\n");
        exit(EXIT_FAILURE);
    }
    glUniformMatrix4fv(shadowTransformsLocation, 6, GL_FALSE, &shadowTransforms[0][0][0]);
}

void Light::update_shadow_transforms()
{
    shadowTransforms[0] = projectionMatrix * lookAt(position, position + vec3(1, 0, 0), vec3(0, -1, 0));
    shadowTransforms[1] = projectionMatrix * lookAt(position, position + vec3(-1, 0, 0), vec3(0, -1, 0));
    shadowTransforms[2] = projectionMatrix * lookAt(position, position + vec3(0, 1, 0), vec3(0, 0, 1));
    shadowTransforms[3] = projectionMatrix * lookAt(position, position + vec3(0, -1, 0), vec3(0, 0, -1));
    shadowTransforms[4] = projectionMatrix * lookAt(position, position + vec3(0, 0, 1), vec3(0, -1, 0));
    shadowTransforms[5] = projectionMatrix * lookAt(position, position + vec3(0, 0, -1), vec3(0, -1, 0));
}