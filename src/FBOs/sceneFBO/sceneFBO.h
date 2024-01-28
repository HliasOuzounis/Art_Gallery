#pragma once

// Include GLEW
#include <GL/glew.h>

#include "src/constants.h"
#include "src/FBOs/FBO.h"

class SceneFBO : public FBO
{
public:
    GLuint colorTexture;
    GLuint rbo;

    SceneFBO();
    void bind();
};