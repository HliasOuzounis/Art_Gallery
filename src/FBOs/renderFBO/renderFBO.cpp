#include "renderFBO.h"

RenderFBO::RenderFBO()
{
    this->fbo = 0;
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    this->viewPortHeight = W_HEIGHT;
    this->viewPortWidth = W_WIDTH;

    checkErrors("renderFBO");
}

void RenderFBO::bind()
{
    FBO::bind();

    glViewport(0, 0, W_WIDTH, W_HEIGHT);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
}