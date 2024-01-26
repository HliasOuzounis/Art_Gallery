#pragma once

#include <GL/glew.h>

#include "src/constants.h"

class Painterly
{
public:
    static void applyPainterly();
static bool useFancyBorders;

private:
static GLuint imagePixels[W_WIDTH * W_HEIGHT];
static int brushColors[W_HEIGHT][W_WIDTH][3];

static constexpr int iterations = 15;
static constexpr int sMax = 32;
static constexpr int sMin = 8;
static constexpr int delta = sMin >> 2;
static constexpr int lambda[3] = {2, 3, 5};
static constexpr float threshold = 0.1;

static void initializeBrushColors();
static void applyIteration();
static void changeColor(int x, int deltaX, int i, int y, int deltaY, int j, int b1);
static void copyToTexture();

// Fancy borders
static int borders[W_HEIGHT][W_WIDTH];
static void edgeDetection();
static void regionColoring();
static void dfs(int startX, int startY, int region_count);
};