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
    virtual void bind();
    
protected:
    void checkErrors(const char *name);
};