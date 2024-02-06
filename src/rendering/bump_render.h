#pragma once

#include <GL/glew.h>
#include "src/constants.h"
#include "src/FBOs/bumpFBO/bumpFBO.h"
#include "rooms/room.h"
#include <common/camera.h>

void initializeBumpRenderLoop();
void bumpPass(BumpFBO *bumpFBO, Camera *camera, Room *currentRoom);