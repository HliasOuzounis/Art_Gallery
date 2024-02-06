#pragma once

// Include GLEW
#include <GL/glew.h>

#include "src/constants.h"
#include "src/FBOs/FBO.h"

class BumpFBO : public FBO
{
public:
    GLuint normalTexture;
    GLuint depthTexture;
    GLuint rbo;

    BumpFBO();
    void addNormalTexture(GLuint &texture);
    void addDepthTexture(GLuint &texture);
    void bind();
};