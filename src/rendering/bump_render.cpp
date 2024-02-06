#include "bump_render.h"
#include <common/shader.h>

GLuint bumpShaderProgram;
GLuint bumpModelMatrixLocation, bumpViewMatrixLocation,
    bumpProjectionMatrixLocation, bumpCameraPositionLocation;

void initializeBumpRenderLoop()
{
    bumpShaderProgram = loadShaders("src/shaders/bump/bump.vertex.glsl", "src/shaders/bump/bump.frag.glsl", nullptr);
    bumpModelMatrixLocation = glGetUniformLocation(bumpShaderProgram, "M");
    bumpViewMatrixLocation = glGetUniformLocation(bumpShaderProgram, "V");
    bumpProjectionMatrixLocation = glGetUniformLocation(bumpShaderProgram, "P");
    bumpCameraPositionLocation = glGetUniformLocation(bumpShaderProgram, "cameraPosition");
}

void bumpPass(BumpFBO *bumpFBO, Camera *camera, Room *currentRoom)
{
    bumpFBO->bind();

    glUseProgram(bumpShaderProgram);

    glUniformMatrix4fv(bumpViewMatrixLocation, 1, GL_FALSE, &camera->viewMatrix[0][0]);
    glUniformMatrix4fv(bumpProjectionMatrixLocation, 1, GL_FALSE, &camera->projectionMatrix[0][0]);
    glUniform3fv(bumpCameraPositionLocation, 1, &camera->position[0]);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cerr << "OpenGL error: " << error << std::endl;
        std::cerr << "Error in uniform " << std::endl;
        cout << bumpCameraPositionLocation << endl;
    }

    currentRoom->render(bumpModelMatrixLocation);

    error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cerr << "OpenGL error: " << error << std::endl;
        std::cerr << "Error in bump pass" << std::endl;
    }
}