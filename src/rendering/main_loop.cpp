#include "main_loop.h"

#include <common/shader.h>

#include "src/FBOs/sceneFBO/sceneFBO.h"
#include "src/FBOs/depthFBO/depthFBO.h"

#include "src/constants.h"

GLuint shaderProgram;
GLuint modelMatrixLocation, viewMatrixLocation, projectionMatrixLocation,
    materialLocation[4], useTextureLocation, viewPosLocation,
    diffuseSamplerLocation, specularSamplerLocation, depthMapLocation;

GLuint depthProgram;
GLuint shadowViewProjectionLocation, shadowModelLocation;

void initializeMainRenderLoop()
{
    shaderProgram = loadShaders("src/shaders/render.vertex.glsl", "src/shaders/render.frag.glsl");

    depthProgram = loadShaders("src/shaders/depth_pass/depth_vertex.glsl",
                               "src/shaders/depth_pass/depth_fragment.glsl",
                               "src/shaders/depth_pass/depth_geometry.glsl");

    // Find uniforms
    modelMatrixLocation = glGetUniformLocation(shaderProgram, "M");
    viewMatrixLocation = glGetUniformLocation(shaderProgram, "V");
    projectionMatrixLocation = glGetUniformLocation(shaderProgram, "P");
    materialLocation[0] = glGetUniformLocation(shaderProgram, "material.Ka");
    materialLocation[1] = glGetUniformLocation(shaderProgram, "material.Kd");
    materialLocation[2] = glGetUniformLocation(shaderProgram, "material.Ks");
    materialLocation[3] = glGetUniformLocation(shaderProgram, "material.shininess");
    useTextureLocation = glGetUniformLocation(shaderProgram, "useTexture");
    viewPosLocation = glGetUniformLocation(shaderProgram, "viewPos");
    diffuseSamplerLocation = glGetUniformLocation(shaderProgram, "diffuseSampler");
    specularSamplerLocation = glGetUniformLocation(shaderProgram, "specularSampler");
    depthMapLocation = glGetUniformLocation(shaderProgram, "depthMap");

    // --- depthProgram ---
    shadowModelLocation = glGetUniformLocation(depthProgram, "M");

}

void depthPass(DepthFBO *depthFBO, Room *currentRoom)
{
    depthFBO->bind();

    glUseProgram(depthProgram);

    // -- render to depth texture -- //
    currentRoom->render(depthProgram, shadowModelLocation);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cerr << "OpenGL error: " << error << std::endl;
        std::cerr << "Error in depth pass" << std::endl;
    }
}

void lightPass(SceneFBO *sceneFBO,Camera *camera, Room *currentRoom, GLuint depthMap)
{
    sceneFBO->bind();

    glUseProgram(shaderProgram);

    glActiveTexture(DEPTH_TEXTURE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthMap);

    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &camera->viewMatrix[0][0]);
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &camera->projectionMatrix[0][0]);
    glUniform3fv(viewPosLocation, 1, &camera->position[0]);

    glUniform1i(diffuseSamplerLocation, DIFFUSE_TEXTURE_LOCATION);
    glUniform1i(specularSamplerLocation, SPECULAR_TEXTURE_LOCATION);
    glUniform1i(depthMapLocation, DEPTH_TEXTURE_LOCATION);

    // Draw currentRoom
    currentRoom->render(shaderProgram, modelMatrixLocation, materialLocation, useTextureLocation);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cerr << "OpenGL error: " << error << std::endl;
        std::cerr << "Error in light pass" << std::endl;
    }
}