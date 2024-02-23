#include "disp_render.h"
#include <common/shader.h>

GLuint dispShaderProgram;
GLuint dispModelMatrixLocation, dispViewMatrixLocation,
    dispProjectionMatrixLocation, dispCameraPositionLocation;

void initializeDispRenderLoop()
{
    dispShaderProgram = loadShaders("src/shaders/displacement/displacement.vertex.glsl", "src/shaders/displacement/displacement.frag.glsl", nullptr);
    dispModelMatrixLocation = glGetUniformLocation(dispShaderProgram, "M");
    dispViewMatrixLocation = glGetUniformLocation(dispShaderProgram, "V");
    dispProjectionMatrixLocation = glGetUniformLocation(dispShaderProgram, "P");
    dispCameraPositionLocation = glGetUniformLocation(dispShaderProgram, "cameraPosition");
}

void displacementPass(PaintingsFBO *displacementFBO, Camera *camera, Room *currentRoom)
{
    displacementFBO->bind();

    glUseProgram(dispShaderProgram);

    glUniformMatrix4fv(dispViewMatrixLocation, 1, GL_FALSE, &camera->viewMatrix[0][0]);
    glUniformMatrix4fv(dispProjectionMatrixLocation, 1, GL_FALSE, &camera->projectionMatrix[0][0]);
    glUniform3fv(dispCameraPositionLocation, 1, &camera->position[0]);

    currentRoom->render(dispModelMatrixLocation);

   
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cerr << "OpenGL error: " << error << std::endl;
        std::cerr << "Error in displacement pass" << std::endl;
    }
}