#pragma once

// Include GLEW
#include <GL/glew.h>

#include "src/constants.h"
#include "src/FBOs/FBO.h"

class DepthFBO : public FBO
{
public:
    GLuint depthCubeMap;

    DepthFBO();
    void bind();
};