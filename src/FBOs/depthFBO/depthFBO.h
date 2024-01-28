#pragma once

// Include GLEW
#include <GL/glew.h>

#include "src/constants.h"

class DepthFBO
{
public:
    GLuint fbo;
    GLuint depthCubeMap;

    int viewPortHeight = SHADOW_HEIGHT;
    int viewPortWidth = SHADOW_WIDTH;

    DepthFBO();
    void checkErrors();
    void bind();
};