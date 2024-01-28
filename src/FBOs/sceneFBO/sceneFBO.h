#pragma once

// Include GLEW
#include <GL/glew.h>

#include "src/constants.h"

class SceneFBO
{
public:
    GLuint fbo;
    GLuint colorTexture;
    GLuint rbo;

    int viewPortHeight = W_HEIGHT;
    int viewPortWidth = W_WIDTH;

    SceneFBO();
    void checkErrors();
    void bind();
};