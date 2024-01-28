#pragma once

// Include GLEW
#include <GL/glew.h>

#include "src/constants.h"
#include "src/FBOs/FBO.h"

class PaintingsFBO : public FBO
{
public:
    GLuint colorTexture;
    GLuint rbo;

    PaintingsFBO();
    void addTexture(GLuint &texture);
    void bind();
};