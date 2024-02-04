#pragma once

#include <GL/glew.h>

#include <common/camera.h>
#include "rooms/room.h"
#include "src/FBOs/sceneFBO/sceneFBO.h"
#include "src/FBOs/depthFBO/depthFBO.h"

void initializeMainRenderLoop();

void depthPass(DepthFBO *depthFBO, Room *currentRoom);
void lightPass(SceneFBO *sceneFBO, Camera *camera, Room *currentRoom, GLuint depthMap); // depthMap is the texture from the depthFBO