#pragma once

// Include GLEW
#include <GL/glew.h>

class FBO
{
public:
    GLuint fbo;

    int viewPortHeight;
    int viewPortWidth;

    FBO(){};
    void checkErrors(const char *name);
    virtual void bind();
};