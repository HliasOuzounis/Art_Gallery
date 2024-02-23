#pragma once

#include <GL/glew.h>
#include "src/constants.h"
#include "src/FBOs/paintingsFBO/paintingsFBO.h"
#include "rooms/room.h"
#include <common/camera.h>

void initializeDispRenderLoop();
void displacementPass(PaintingsFBO *displacementFBO, Camera *camera, Room *currentRoom);