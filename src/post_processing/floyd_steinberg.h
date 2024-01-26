#pragma once

#include <GL/glew.h>

#include "src/constants.h"

class FloydSteinbergDithering
{
public:
    static void applyDithering(int colors);

private:
    static GLuint imagePixels[W_WIDTH * W_HEIGHT];
    static int pixelError[W_HEIGHT][W_WIDTH][3];
};