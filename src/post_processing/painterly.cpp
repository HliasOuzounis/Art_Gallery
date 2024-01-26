#include <random>
#include <stack>

#include "painterly.h"

GLuint Painterly::imagePixels[W_WIDTH * W_HEIGHT];
int Painterly::brushColors[W_HEIGHT][W_WIDTH][3];
int Painterly::borders[W_HEIGHT][W_WIDTH];

bool Painterly::useFancyBorders = false;

void Painterly::applyPainterly()
{
    // https://arxiv.org/pdf/0911.4874.pdf

    glReadPixels(0, 0, W_WIDTH, W_HEIGHT, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, imagePixels);
    // pixel is of the form 0xAARRGGBB

    srand(imagePixels[0] + imagePixels[W_WIDTH * W_HEIGHT - 1] + imagePixels[W_WIDTH * W_HEIGHT / 2]);

    initializeBrushColors();

    if (useFancyBorders)
    {
        edgeDetection();
        regionColoring();
    }

    for (int iter = 0; iter < iterations; iter++)
    {
        applyIteration();
    }
    copyToTexture();
}

void Painterly::applyIteration()
{
    int s = rand() % sMax + sMin;
    for (int y = s; y < W_HEIGHT; y += s)
    {
        for (int x = s; x < W_WIDTH; x += s)
        {
            int deltaX = rand() % (2 * delta) - delta;
            int deltaY = rand() % (2 * delta) - delta;

            if (x + deltaX < 0 || y + deltaY < 0 || x + deltaX >= W_WIDTH || y + deltaY > W_HEIGHT ||
                x + deltaX + lambda[1] >= W_WIDTH || y + deltaY + lambda[1] >= W_HEIGHT)
                continue;

            int b1 = brushColors[y + deltaY][x + deltaX][0];
            int b2 = brushColors[y + deltaY + lambda[1]][x + deltaX][0];
            int b3 = brushColors[y + deltaY][x + deltaX + lambda[1]][0];

            if (b1 == 0)
                continue;

            float A = abs(b2 - b1) / (float)b1;
            float B = abs(b3 - b1) / (float)b1;

            int d1 = A > threshold ? lambda[0] : lambda[2];
            int d2 = B > threshold ? lambda[0] : lambda[2];

            if (A <= threshold && B <= threshold)
            {
                d1 = lambda[1];
                d2 = lambda[1];
            }

            for (int i = -d1; i <= d1; i++)
            {
                for (int j = -d2; j <= d2; j++)
                {
                    changeColor(x, deltaX, i, y, deltaY, j, b1);
                }
            }
        }
    }
}

void Painterly::initializeBrushColors()
{
    for (int y = 0; y < W_HEIGHT; y++)
    {
        for (int x = 0; x < W_WIDTH; x++)
        {
            int pixelIndex = y * W_WIDTH + x;
            GLuint oldColor = imagePixels[pixelIndex];
            brushColors[y][x][0] = (oldColor >> 16) & 0xFF;
            brushColors[y][x][1] = (oldColor >> 8) & 0xFF;
            brushColors[y][x][2] = oldColor & 0xFF;
        }
    }
}
void Painterly::copyToTexture()
{
    for (int y = 0; y < W_HEIGHT; y++)
    {
        for (int x = 0; x < W_WIDTH; x++)
        {
            int pixelIndex = y * W_WIDTH + x;
            imagePixels[pixelIndex] = brushColors[y][x][2] | (brushColors[y][x][1] << 8) |
                                      (brushColors[y][x][0] << 16) | (imagePixels[pixelIndex] & 0xFF000000);
        }
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, W_WIDTH, W_HEIGHT, 0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, imagePixels);
}

void Painterly::changeColor(int x, int deltaX, int i, int y, int deltaY, int j, int b1)
{
    static float threshold = 1;

    int w = x + deltaX + i;
    int h = y + deltaY + j;
    if (w < 0 || w >= W_WIDTH || h < 0 || h >= W_HEIGHT)
    {
        return;
    }
    if (useFancyBorders && borders[h][w] != borders[y + deltaY][x + deltaX])
    {
        return;
    }

    float C = abs(brushColors[h][w][0] - b1) / (float)b1;
    if (C > threshold)
    {
        return;
    }
    brushColors[h][w][0] = brushColors[y + deltaY][x + deltaX][0];
    brushColors[h][w][1] = brushColors[y + deltaY][x + deltaX][1];
    brushColors[h][w][2] = brushColors[y + deltaY][x + deltaX][2];
}

void Painterly::edgeDetection()
{
    // Sobel operator for edge detection
    static const int sobelX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    static const int sobelY[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int y = 0; y < W_HEIGHT; y++)
    {
        for (int x = 0; x < W_WIDTH; x++)
        {
            int sobelXValue = 0;
            int sobelYValue = 0;

            for (int k = -1; k <= 1; k++)
            {
                for (int l = -1; l <= 1; l++)
                {
                    int w = x + k;
                    int h = y + l;
                    if (w < 0 || w >= W_WIDTH || h < 0 || h >= W_HEIGHT)
                    {
                        continue;
                    }
                    for (int c = 0; c < 3; c++)
                    {
                        sobelXValue += brushColors[h][w][c] * sobelX[k + 1][l + 1];
                        sobelYValue += brushColors[h][w][c] * sobelY[k + 1][l + 1];
                    }
                }
            }
            int magnitude = sqrt(sobelXValue * sobelXValue + sobelYValue * sobelYValue);

            borders[y][x] = magnitude > 900 ? 1 : 0;
        }
    }
}

void Painterly::regionColoring()
{
    int region_count = 2;
    for (int y = 0; y < W_HEIGHT; y++)
    {
        for (int x = 0; x < W_WIDTH; x++)
        {
            if (borders[y][x] != 0)
            {
                continue;
            }
            dfs(x, y, region_count);
            region_count++;
        }
    }
}

void Painterly::dfs(int startX, int startY, int color)
{
    std::stack<std::pair<int, int>> stack;
    stack.push(std::make_pair(startX, startY));

    while (!stack.empty())
    {
        std::pair<int, int> current = stack.top();
        stack.pop();

        int x = current.first;
        int y = current.second;

        if (x < 0 || x >= W_WIDTH || y < 0 || y >= W_HEIGHT || borders[y][x] != 0)
        {
            continue;
        }

        borders[y][x] = color;

        // Define offsets for neighboring pixels
        static const int dx[] = {1, 0, -1, 0, 1, 1, -1, -1};
        static const int dy[] = {0, 1, 0, -1, 1, -1, 1, -1};

        // Push neighboring pixels onto the stack
        for (int i = 0; i < 8; i++)
        {
            int w = x + dx[i];
            int h = y + dy[i];

            if (w >= 0 && w < W_WIDTH && h >= 0 && h < W_HEIGHT && borders[h][w] == 0)
            {
                stack.push(std::make_pair(w, h));
            }
        }
    }
}