#include <algorithm>

#include "floyd_steinberg.h"

GLuint FloydSteinbergDithering::imagePixels[W_WIDTH * W_HEIGHT];
int FloydSteinbergDithering::pixelError[W_HEIGHT][W_WIDTH][3];

void FloydSteinbergDithering::applyDithering(int colors)
{
    glReadPixels(0, 0, W_WIDTH, W_HEIGHT, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, imagePixels);
    // pixel is of the form 0xAARRGGBB

    int quantization = 256 / colors;
    float inv_quantization = 1.0f / quantization;

    for (int y = 0; y < W_HEIGHT; y++)
    {
        for (int x = 0; x < W_WIDTH; x++)
        {
            int pixelIndex = y * W_WIDTH + x;
            GLuint oldColor = imagePixels[pixelIndex];
            int oldR = pixelError[y][x][0] + ((oldColor >> 16) & 0xFF);
            int oldG = pixelError[y][x][1] + ((oldColor >> 8) & 0xFF);
            int oldB = pixelError[y][x][2] + (oldColor & 0xFF);
            int oldA = (oldColor >> 24) & 0xFF;

            int newR = std::min(int(oldR * inv_quantization) * quantization, 255);
            int newG = std::min(int(oldG * inv_quantization) * quantization, 255);
            int newB = std::min(int(oldB * inv_quantization) * quantization, 255);

            imagePixels[pixelIndex] = newB | (newG << 8) | (newR << 16) | (oldA << 24);

            int quantizationErrorR = oldR - newR;
            int quantizationErrorG = oldG - newG;
            int quantizationErrorB = oldB - newB;

            if (x + 1 < W_WIDTH)
            {
                // (>> 4) == (/ 16) but faster
                pixelError[y][x + 1][0] += quantizationErrorR * 7 >> 4;
                pixelError[y][x + 1][1] += quantizationErrorG * 7 >> 4;
                pixelError[y][x + 1][2] += quantizationErrorB * 7 >> 4;
            }
            if (x - 1 >= 0 && y + 1 < W_HEIGHT)
            {
                pixelError[y + 1][x - 1][0] += quantizationErrorR * 3 >> 4;
                pixelError[y + 1][x - 1][1] += quantizationErrorG * 3 >> 4;
                pixelError[y + 1][x - 1][2] += quantizationErrorB * 3 >> 4;
            }
            if (y + 1 < W_HEIGHT)
            {
                pixelError[y + 1][x][0] += quantizationErrorR * 5 >> 4;
                pixelError[y + 1][x][1] += quantizationErrorG * 5 >> 4;
                pixelError[y + 1][x][2] += quantizationErrorB * 5 >> 4;
            }
            if (x + 1 < W_WIDTH && y + 1 < W_HEIGHT)
            {
                pixelError[y + 1][x + 1][0] += quantizationErrorR >> 4;
                pixelError[y + 1][x + 1][1] += quantizationErrorG >> 4;
                pixelError[y + 1][x + 1][2] += quantizationErrorB >> 4;
            }

            pixelError[y][x][0] = 0;
            pixelError[y][x][1] = 0;
            pixelError[y][x][2] = 0;
        }
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, W_WIDTH, W_HEIGHT, 0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, FloydSteinbergDithering::imagePixels);
}