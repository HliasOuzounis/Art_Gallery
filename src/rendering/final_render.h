#pragma once

#include "src/FBOs/FBO.h"
#include <GL/glew.h>
#include "src/constants.h"

void initializeFinalRender();
void displayScene(FBO *fbo, GLuint texture, GameState gameState);
void applyPostProcessing(GameState gameState);