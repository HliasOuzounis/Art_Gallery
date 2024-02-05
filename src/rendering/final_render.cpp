#include "final_render.h"

#include "src/post_processing/floyd_steinberg.h"
#include "src/post_processing/painterly.h"

#include <common/shader.h>
#include <common/model.h>

#include <vector>
#include <glm/glm.hpp>
using namespace glm;

#include <iostream>
#include <GLFW/glfw3.h>
using namespace std;

Drawable *quad;
GLuint postProcessingProgram[PAINTINGS + 1], quadTextureSamplerLocation[PAINTINGS + 1];

void initializeFinalRender()
{
    GLuint defaultShader = loadShaders("src/shaders/image_processing/vertex.glsl",
                                       "src/shaders/image_processing/default.frag.glsl");

    postProcessingProgram[MAINROOM] = defaultShader;

    postProcessingProgram[ROOM1] = defaultShader;

    postProcessingProgram[ROOM2] = defaultShader;

    postProcessingProgram[ROOM3] = defaultShader;

    postProcessingProgram[ROOM4] = loadShaders("src/shaders/image_processing/vertex.glsl",
                                               "src/shaders/image_processing/fish_eye.frag.glsl");
    postProcessingProgram[ROOM5] = loadShaders("src/shaders/image_processing/vertex.glsl",
                                               "src/shaders/image_processing/toon.frag.glsl");
    postProcessingProgram[ROOM6] = loadShaders("src/shaders/image_processing/vertex.glsl",
                                               "src/shaders/image_processing/chromatic_aberration.frag.glsl");
    for (int i = 0; i < PAINTINGS + 1; i++)
    {
        quadTextureSamplerLocation[i] = glGetUniformLocation(postProcessingProgram[i], "screenTexture");
    }

    vector<vec3> quadVertices = {
        vec3(-1.0f, 1.0f, 0.0f),  // top left
        vec3(-1.0f, -1.0f, 0.0f), // bottom left
        vec3(1.0f, -1.0f, 0.0f),  // bottom right

        vec3(-1.0f, 1.0f, 0.0f), // top left
        vec3(1.0f, -1.0f, 0.0f), // bottom right
        vec3(1.0f, 1.0f, 0.0f)   // top right
    };
    vector<vec2> quadUVs = {
        vec2(0.0f, 1.0f), // top left
        vec2(0.0f, 0.0f), // bottom left
        vec2(1.0f, 0.0f), // bottom right

        vec2(0.0f, 1.0f), // top left
        vec2(1.0f, 0.0f), // bottom right
        vec2(1.0f, 1.0f)  // top right
    };
    vector<vec3> quadNormals = {
        vec3(0.0f, 0.0f, 1.0f), // top left
        vec3(0.0f, 0.0f, 1.0f), // bottom left
        vec3(0.0f, 0.0f, 1.0f), // bottom right

        vec3(0.0f, 0.0f, 1.0f), // top left
        vec3(0.0f, 0.0f, 1.0f), // bottom right
        vec3(0.0f, 0.0f, 1.0f)  // top right
    };
    quad = new Drawable(quadVertices, quadUVs, quadNormals);
}

void displayScene(FBO *fbo, GLuint texture, GameState gameState)
{
    glUseProgram(postProcessingProgram[gameState]);

    glActiveTexture(DIFFUSE_TEXTURE);
    glBindTexture(GL_TEXTURE_2D, texture);

    applyPostProcessing(gameState);

    fbo->bind();

    glUniform1i(quadTextureSamplerLocation[gameState], DIFFUSE_TEXTURE_LOCATION);
    quad->bind();
    quad->draw();

    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        glewGetErrorString(error);
        std::cerr << "OpenGL error: " << glewGetErrorString(error) << std::endl;
        std::cerr << "Error in display scene pass" << std::endl;
    }
}

void applyPostProcessing(GameState gameState){
    GLuint timeLocation;
    const int colors = 2;
    switch (gameState)
    {
    case ROOM2:
        FloydSteinbergDithering::applyDithering(colors);
        break;
    case ROOM3:
        Painterly::applyPainterly();
        break;
    case ROOM6:
        timeLocation = glGetUniformLocation(postProcessingProgram[gameState], "time");
        glUniform1f(timeLocation, (float)glfwGetTime());
        break;
    default:
        break;
    }
}