#include <stdio.h>
#include <stdlib.h>
#include "t_bmp8.h"
#include "bmp8.c"
#include "t_bmp24.h"
#include "bmp24.c"


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

    //Box  Blur
    image = bmp8_loadImage("images/lena_gray.bmp");
    float **kernel = createBoxBlurKernel();
    bmp8_applyFilter(image, kernel, 3);
    bmp8_saveImage("images/lena_boxblur.bmp", image);
    freeKernel(kernel, 3);
    bmp8_free(image);


    //Gaussian Blur
    image = bmp8_loadImage("images/lena_gray.bmp");
    kernel = createGaussianBlurKernel();
    bmp8_applyFilter(image, kernel, 2);
    bmp8_saveImage("images/lena_gaussian.bmp", image);
    freeKernel(kernel, 3);
    bmp8_free(image);

    // Outline
    image = bmp8_loadImage("images/lena_gray.bmp");
    kernel = createOutlineKernel();
    bmp8_applyFilter(image, kernel, 3);
    bmp8_saveImage("images/lena_outline.bmp", image);
    freeKernel(kernel, 3);
    bmp8_free(image);

    // Emboss
    image = bmp8_loadImage("images/lena_gray.bmp");
    kernel = createEmbossKernel();
    bmp8_applyFilter(image, kernel, 3);
    bmp8_saveImage("images/lena_emboss.bmp", image);
    freeKernel(kernel, 3);
    bmp8_free(image);

    // Sharpen
    image = bmp8_loadImage("images/lena_gray.bmp");
    kernel = createSharpenKernel();
    bmp8_applyFilter(image, kernel, 3);
    bmp8_saveImage("images/lena_sharpen.bmp", image);
    freeKernel(kernel, 3);
    bmp8_free(image);

    // Partie 2 : Traitement de l'image 24 bits (couleur)
    t_bmp24 *image24 = bmp24_loadImage("images/flowers_color.bmp");
    // Vérifie que l'image 24 bits a bien été chargée
    if (image24 == NULL) {
        printf("Erreur : Impossible de charger l'image 24 bits.\n");
        return 1;
    }

    // Affiche les informations de l'image 24 bits
    printf("Informations de l'image 24 bits :\n");
    printf("Largeur : %d\n", image24->width);
    printf("Hauteur : %d\n", image24->height);
    printf("Profondeur de couleur : %d\n", image24->colorDepth);
    bmp24_saveImage(image24, "images/copie_flowers_color.bmp"); // Sauvegarde

    // Applique le négatif sur l'image 24 bits
    bmp24_negative(image24);
    bmp24_saveImage(image24, "images/flowers_color_negatif.bmp"); // Sauvegarde

    // Applique de la luminosité, en ajoutant +50 pixels (image 24 bits)
    bmp24_brightness(image24, 50);
    bmp24_saveImage(image24, "images/flowers_color_bright.bmp"); // Sauvegarde

    // Transforme l'image en niveaux de gris (image 24 bits)
    bmp24_grayscale(image24);
    bmp24_saveImage(image24, "images/flowers_color_gray.bmp"); // Sauvegarde


    /// Filtres de convolution ///


    //applique Box Blur
    bmp24_boxBlur(image24);
    bmp24_saveImage(image24, "images/flowers_color_boxblur.bmp");
    bmp24_free(image24);

    //applique Gaussian Blur
    image24 = bmp24_loadImage("images/flowers_color.bmp");
    bmp24_gaussianBlur(image24);
    bmp24_saveImage(image24, "images/flowers_color_gaussian.bmp");
    bmp24_free(image24);

    //applique Outline
    image24 = bmp24_loadImage("images/flowers_color.bmp");
    bmp24_outline(image24);
    bmp24_saveImage(image24, "images/flowers_color_outline.bmp");
    bmp24_free(image24);

    //applique Emboss
    image24 = bmp24_loadImage("images/flowers_color.bmp");
    bmp24_emboss(image24);
    bmp24_saveImage(image24, "images/flowers_color_emboss.bmp");
    bmp24_free(image24);

    //applique Sharpen
    image24 = bmp24_loadImage("images/flowers_color.bmp");
    bmp24_sharpen(image24);
    bmp24_saveImage(image24, "images/flowers_color_sharpen.bmp");
    bmp24_free(image24);


    // Libère la mémoire pour l'image 24 bits
    bmp24_free(image24);
    return 0;
}