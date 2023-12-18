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
    projectionMatrix = ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    modelMatrix = translate(modelMatrix, position) * scale(mat4(1.0f), vec3(0.1f));
    this->drawable = new Drawable("src/extra/light/models/sphere.obj");
}

void Light::upload_to_shaders(GLuint shaderProgram)
{
    GLuint lightPositionLocation = glGetUniformLocation(shaderProgram, "light.lightPosition");
    if (lightPositionLocation == -1)
    {
        fprintf(stderr, "Error: can't find light position uniforms\n");
        exit(EXIT_FAILURE);
    }
    glUniform3f(lightPositionLocation, position.x, position.y, position.z);

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
    }
    glUniform4f(LsLocation, color.r, color.g, color.b, 1.0f);

    /*/
    GLuint lightIntensityLocation = glGetUniformLocation(shaderProgram, "light.lightIntensity");
    if (lightIntensityLocation == -1)
    {
        fprintf(stderr, "Error: can't find light intensity uniforms\n");
        exit(EXIT_FAILURE);
    }
    glUniform1f(lightIntensityLocation, intensity);
    //*/
    /*/
    mat4 lightSpaceMatrix = get_light_space_matrix();
    GLuint lightVPLocation = glGetUniformLocation(shaderProgram, "light.lightVP");
    if (lightVPLocation == -1)
    {
        fprintf(stderr, "Error: can't find light VP matrix uniforms\n");
        // exit(EXIT_FAILURE);
    }
    glUniformMatrix4fv(lightVPLocation, 1, GL_FALSE, &lightSpaceMatrix[0][0]);
    //*/
}

mat4 Light::get_light_space_matrix()
{
    viewMatrix = lookAt(position, vec3(0, 0, 0), vec3(0, 1, 0));
    return projectionMatrix * viewMatrix;
}