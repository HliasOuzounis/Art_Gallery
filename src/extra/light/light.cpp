#include <common/model.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

using namespace glm;
using namespace std;

#include <iostream>
#include "light.h"

Light::Light(vec3 position, vec4 color, float intensity, float radius)
    : position(position), color(color), intensity(intensity), radius(radius)
{
    direction = normalize(vec3(0, 0, 0) - position);
    projectionMatrix = ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
}

void Light::upload_to_shaders(GLuint shaderProgram)
{
    GLuint lightPositionLocation = glGetUniformLocation(shaderProgram, "lightPosition");
    GLuint lightColorLocation = glGetUniformLocation(shaderProgram, "lightColor");
    // GLuint lightIntensityLocation = glGetUniformLocation(shaderProgram, "lightIntensity");

    if (lightPositionLocation == -1 || lightColorLocation == -1)
    {
        fprintf(stderr, "Error: can't find light uniforms\n");
        exit(EXIT_FAILURE);
    }

    glUniform3fv(lightPositionLocation, 1, &position[0]);
    glUniform4fv(lightColorLocation, 1, &color[0]);
    // glUniform1f(lightIntensityLocation, intensity);

    GLuint lightSpaceMatrixLocation = glGetUniformLocation(shaderProgram, "lightVP");
    mat4 lightSpaceMatrix = get_light_space_matrix();
    glUniformMatrix4fv(lightSpaceMatrixLocation, 1, GL_FALSE, &lightSpaceMatrix[0][0]);
}

mat4 Light::get_light_space_matrix()
{
    viewMatrix = lookAt(position, vec3(0, 0, 0), vec3(0, 1, 0));
    return projectionMatrix * viewMatrix;
}