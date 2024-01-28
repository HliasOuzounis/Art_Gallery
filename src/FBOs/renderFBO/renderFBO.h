#pragma once

// Include GLEW
#include <GL/glew.h>

#include "src/constants.h"
#include "src/FBOs/FBO.h"

class RenderFBO : public FBO
{
public:
    GLuint colorTexture;
    GLuint rbo;

    RenderFBO();
    void addTexture(GLuint &texture);
    void bind();
};