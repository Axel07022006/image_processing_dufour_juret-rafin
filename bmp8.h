#ifndef T_BMP8_H
#define T_BMP8_H

//structure pour représenter une image BMP 8 bits
typedef struct{
    unsigned char header[54];
    unsigned char colorTable[1024];
    unsigned char * data;

    unsigned int width;
    unsigned int height;
    unsigned int colorDepth;
    unsigned int dataSize;
}t_bmp8;


//ensemble des prototypes du fichier bmp8.c
t_bmp8 * bmp8_loadImage(const char * filename);
void bmp8_saveImage(const char * filename, t_bmp8 * img);
void bmp8_free(t_bmp8 * img);
void bmp8_printInfo(t_bmp8 * img);
void bmp8_negative(t_bmp8 * img);
void bmp8_brightness(t_bmp8 * img, int value);
void bmp8_threshold(t_bmp8 * img, int threshold);
void bmp8_applyFilter(t_bmp8 *img, float **kernel, int kernelSize);
float **createKernel(float values[3][3], float divisor);
void freeKernel(float **kernel, int size);
float **createBoxBlurKernel();
float **createGaussianBlurKernel();
float **createOutlineKernel();
float **createEmbossKernel();
float **createSharpenKernel();

unsigned int * bmp8_computeHistogram(t_bmp8 * img);
unsigned int * bmp8_computeCDF(unsigned int * hist);
void bmp8_equalize(t_bmp8 * img, unsigned int * hist_eq);

#endif //T_BMP8_H
