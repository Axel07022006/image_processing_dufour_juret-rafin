#ifndef BMP8_H
#define BMP8_H


typedef struct{
    unsigned char header[54];
    unsigned char colorTable[1024];
    unsigned char * data;

    unsigned int width;
    unsigned int height;
    unsigned int colorDepth;
    unsigned int dataSize;
}t_bmp8;

void bmp8_printInfo(t_bmp8 *image);

#endif //BMP8_H