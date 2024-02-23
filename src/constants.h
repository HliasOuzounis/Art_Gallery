#pragma once

#define W_WIDTH 1200
#define W_HEIGHT 900
#define TITLE "Art Gallery"

#define SHADOW_WIDTH 1024
#define SHADOW_HEIGHT 1024

#define PAINTINGS 6

#define DIFFUSE_TEXTURE GL_TEXTURE0
#define SPECULAR_TEXTURE GL_TEXTURE1
#define BUMP_TEXTURE GL_TEXTURE2
#define DISPLACEMENT_TEXTURE GL_TEXTURE3
#define DEPTH_TEXTURE GL_TEXTURE4

#define DIFFUSE_TEXTURE_LOCATION 0
#define SPECULAR_TEXTURE_LOCATION 1
#define BUMP_TEXTURE_LOCATION 2
#define DISPLACEMENT_TEXTURE_LOCATION 3
#define DEPTH_TEXTURE_LOCATION 4

enum GameState
{
    MAINROOM = 0,
    ROOM1 = 1,
    ROOM2 = 2,
    ROOM3 = 3,
    ROOM4 = 4,
    ROOM5 = 5,
    ROOM6 = 6,
};