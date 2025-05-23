//
// Created by mat18 on 23/05/2025.
//
#include <stdint.h>
#include <stdio.h>
#include <math.h>

void rgb_to_yuv(uint8_t R, uint8_t G, uint8_t B, float *Y, float *U, float *V) {
    *Y = 0.299f * R + 0.587f * G + 0.114f * B;
    *U = -0.14713f * R - 0.28886f * G + 0.436f * B;
    *V = 0.615f * R - 0.51499f * G - 0.10001f * B;
}

void yuv_to_rgb(float Y, float U, float V, uint8_t *R, uint8_t *G, uint8_t *B) {
    int r = (int)round(Y + 1.13983f * V);
    int g = (int)round(Y - 0.39465f * U - 0.58060f * V);
    int b = (int)round(Y + 2.03211f * U);

    // Clamp les valeurs dans [0, 255]
    *R = (r < 0) ? 0 : (r > 255) ? 255 : r;
    *G = (g < 0) ? 0 : (g > 255) ? 255 : g;
    *B = (b < 0) ? 0 : (b > 255) ? 255 : b;
}
