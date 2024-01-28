#include "FBO.h"

#include <iostream>
#include <stdexcept>
#include <glfw3.h>

void FBO::checkErrors(const char *name)
{
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cerr << "OpenGL error: " << error << std::endl;
        std::cerr << "Error in " << name << std::endl;
    }

    // now that we have created the framebuffer and added all attachments we want to check if it is actually complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        glfwTerminate();
        throw std::runtime_error("Frame buffer not initialized correctly");
    }
}

void FBO::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, viewPortWidth, viewPortHeight);
};