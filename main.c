#include <stdio.h>
#include <stdlib.h>
#include "quotes.h"
#include "bmp8.c"



int main() {
    // Charge l'image BMP en niveaux de gris
    t_bmp8 * image = bmp8_loadImage("images/lena_gray.bmp");
    // verifie que l'image a bien été chargée
    if (image == NULL) {
        printf("Erreur : Impossible de charger l'image.\n");
        return 1;
    }

    // affiche les informations de l'image
    bmp8_printInfo(image);
    bmp8_saveImage("images/copie_lena.bmp", image);// Sauvegarde

    // APPLIQUE LE NEGATIF
    bmp8_negative(image);
    bmp8_saveImage("images/lena_negatif.bmp", image); // Sauvegarde

    // APPLIQUE DE LA LUMINOSITE, EN AJOUTANT +50 PIXEL
    bmp8_brightness(image, 50);
    bmp8_saveImage("images/lena_bright.bmp", image);// Sauvegarde

    // TRANSFORME L'IMAGE EN UNE IMAGE BINAIRE
    bmp8_threshold(image,128);
    bmp8_saveImage("images/lena_binary.bmp", image); // Sauvegarde

    // ----- Box Blur -----
    image = bmp8_loadImage("images/lena_gray.bmp");
    float **kernel = createBoxBlurKernel();
    bmp8_applyFilter(image, kernel, 3);
    bmp8_saveImage("images/lena_boxblur.bmp", image);
    freeKernel(kernel, 3);
    bmp8_free(image);


    // ----- Gaussian Blur -----
    image = bmp8_loadImage("images/lena_gray.bmp");
    kernel = createGaussianBlurKernel();
    bmp8_applyFilter(image, kernel, 2);
    bmp8_saveImage("images/lena_gaussian.bmp", image);
    freeKernel(kernel, 3);
    bmp8_free(image);

    // ----- Outline -----
    image = bmp8_loadImage("images/lena_gray.bmp");
    kernel = createOutlineKernel();
    bmp8_applyFilter(image, kernel, 3);
    bmp8_saveImage("images/lena_outline.bmp", image);
    freeKernel(kernel, 3);
    bmp8_free(image);

    // ----- Emboss -----
    image = bmp8_loadImage("images/lena_gray.bmp");
    kernel = createEmbossKernel();
    bmp8_applyFilter(image, kernel, 3);
    bmp8_saveImage("images/lena_emboss.bmp", image);
    freeKernel(kernel, 3);
    bmp8_free(image);

    // ----- Sharpen -----
    image = bmp8_loadImage("images/lena_gray.bmp");
    kernel = createSharpenKernel();
    bmp8_applyFilter(image, kernel, 3);
    bmp8_saveImage("images/lena_sharpen.bmp", image);
    freeKernel(kernel, 3);
    bmp8_free(image);

    return 0;
}