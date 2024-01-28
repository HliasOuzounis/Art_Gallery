#pragma once

// Include GLEW
#include <GL/glew.h>

#include "src/constants.h"

class PaintingsFBO
{
public:
    GLuint fbo;
    GLuint colorTexture;
    GLuint rbo;

    int viewPortHeight = W_HEIGHT;
    int viewPortWidth = W_WIDTH;

    PaintingsFBO();
    void addTexture(GLuint &texture);
    void checkErrors();
    void bind();
};