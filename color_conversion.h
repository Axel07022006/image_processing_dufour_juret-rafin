#ifndef CONVERSION_COULEUR_H
#define CONVERSION_COULEUR_H

#include <stdint.h>

void rgb_to_yuv(uint8_t R, uint8_t G, uint8_t B, float *Y, float *U, float *V);
void yuv_to_rgb(float Y, float U, float V, uint8_t *R, uint8_t *G, uint8_t *B);

#endif //CONVERSION_COULEUR_H
