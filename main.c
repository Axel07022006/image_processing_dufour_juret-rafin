#include <stdio.h>
#include <stdlib.h>
#include "bmp8.h"
#include "bmp24.h"


int main() {

    // Charge l'image BMP en niveaux de gris
    t_bmp8 * image = bmp8_loadImage("../images/lena_gray.bmp");
    t_bmp24 *image24 = bmp24_loadImage("../images/flowers_color.bmp");
    //t_bmp8 * image = bmp8_loadImage("barbara_gray.bmp");
    int choix=6;
    while (choix!=5 ) {
        printf("Veuillez choisir une option :\n");
        printf("\t1. Ouvrir une image\n");
        printf("\t2. Sauvegarder une image\n");
        printf("\t3. Appliquer un filtre\n");
        printf("\t4. Afficher les informations de l'image\n");
        printf("\t5. Quitter\n");
        printf("Choisissez parmi ces choix : ");
        scanf("%d",&choix);
        printf("Votre choix : %d",choix);
        switch (choix) {
            case 1:
                int im_choix=3;
                while (im_choix>2) {
                    printf("Choisissez une image parmi les deux suivantes\n");
                    printf("1. Image de Lena_gray\n");
                    printf("2. Images de fleurs\n");
                    scanf("%d",&im_choix);

                }
                if (im_choix==1) {
                    char chemin_lena[]="../images/lena_gray.bmp";
                    printf("Chemin du fichier : %s",chemin_lena);
                    image = bmp8_loadImage("../images/lena_gray.bmp");
                    printf("Image chargée avec succès !");
                }
                if (im_choix==2) {
                    char chemin_flowers[]="../images/flowers_color.bmp";
                    printf("Chemin du fichier :%s",chemin_flowers);
                    image24 = bmp24_loadImage("../images/flowers_color.bmp");
                    printf("Image chargée avec succès !");
                }
            printf("Veuillez choisir une option :\n");
            break;
            case 2:
                bmp8_saveImage("../images/copie_lena.bmp", image);
                printf("Image sauvagardee avec succès !");
                printf("Veuillez choisir une option :\n");
            break;
            case 3:
                int filtre_choix=10;
                while (filtre_choix>9) {
                    printf("Veuillez choisir un filtre :\n");
                    printf("\t 1. Negatif \n");
                    printf("\t 2. Luminosite\n");
                    printf("\t 3. Binarisation \n");
                    printf("\t 4. Flou \n");
                    printf("\t 5. Flou gaussien \n");
                    printf("\t 6. Nettete \n");
                    printf("\t 7. Contours \n");
                    printf("\t 8. Relief \n");
                    printf("\t 9. Retourner au menu precedent \n");
                    scanf("%d",&filtre_choix);
                }
                if (im_choix==1) {
                    switch (filtre_choix) {
                        case 1:
                            bmp8_negative(image);
                            bmp8_saveImage("../images/lena_negatif.bmp", image);
                        break;
                        case 2:
                            bmp8_brightness(image, 50);
                            bmp8_saveImage("../images/lena_bright.bmp", image);
                        break;
                        case 3:
                            // TRANSFORME L'IMAGE EN UNE IMAGE BINAIRE
                            bmp8_threshold(image,128);
                            bmp8_saveImage("../images/lena_binary.bmp", image); // Sauvegarde
                        break;
                        case 4:
                            image = bmp8_loadImage("../images/lena_gray.bmp");
                            float **kernel = createBoxBlurKernel();
                            bmp8_applyFilter(image, kernel, 3);
                            bmp8_saveImage("../images/lena_boxblur.bmp", image);
                            freeKernel(kernel, 3);
                            bmp8_free(image);
                        break;
                        case 5:
                            //Gaussian Blur
                            image = bmp8_loadImage("../images/lena_gray.bmp");
                            kernel = createGaussianBlurKernel();
                            bmp8_applyFilter(image, kernel, 2);
                            bmp8_saveImage("../images/lena_gaussian.bmp", image);
                            freeKernel(kernel, 3);
                            bmp8_free(image);
                        break;
                        case 6:
                            // Emboss
                            image = bmp8_loadImage("../images/lena_gray.bmp");
                            kernel = createEmbossKernel();
                            bmp8_applyFilter(image, kernel, 3);
                            bmp8_saveImage("../images/lena_emboss.bmp", image);
                            freeKernel(kernel, 3);
                            bmp8_free(image);
                        break;
                        case 7:
                            // Outline
                            image = bmp8_loadImage("../images/lena_gray.bmp");
                            kernel = createOutlineKernel();
                            bmp8_applyFilter(image, kernel, 3);
                            bmp8_saveImage("../images/lena_outline.bmp", image);
                            freeKernel(kernel, 3);
                            bmp8_free(image);
                        break;
                        case 8:
                            // Sharpen
                            image = bmp8_loadImage("../images/lena_gray.bmp");
                            kernel = createSharpenKernel();
                            bmp8_applyFilter(image, kernel, 3);
                            bmp8_saveImage("../images/lena_sharpen.bmp", image);
                            freeKernel(kernel, 3);
                            bmp8_free(image);
                        break;
                        case 9:
                            printf("Retourner au menu_precedent :\n");
                        break;
                    }
                    printf("Filtre applique avec succes !\n");
                    printf("Veuillez choisir une option :\n");
                }
                if (im_choix==2) {
                    switch (filtre_choix) {
                        case 1:
                            bmp24_negative(image24);
                            bmp24_saveImage(image24, "../images/flowers_color_negatif.bmp"); // Sauvegarde
                        break;
                        case 2:
                            bmp24_brightness(image24, 50);
                            bmp24_saveImage(image24, "../images/flowers_color_bright.bmp"); // Sauvegarde
                        break;
                        case 3:
                            bmp24_grayscale(image24);
                            bmp24_saveImage(image24, "../images/flowers_color_gray.bmp"); // Sauvegarde
                        break;
                        case 4:
                            bmp24_boxBlur(image24);
                            bmp24_saveImage(image24, "../images/flowers_color_boxblur.bmp");
                        break;
                        case 5:
                            bmp24_gaussianBlur(image24);
                            bmp24_saveImage(image24, "../images/flowers_color_gaussian.bmp");
                        break;
                        case 6:
                            bmp24_saveImage(image24, "../images/flowers_color_outline.bmp");
                            bmp24_emboss(image24);
                        break;
                        case 7:
                            bmp24_outline(image24);
                            bmp24_saveImage(image24, "../images/flowers_color_outline.bmp");
                        break;
                        case 8:
                            bmp24_sharpen(image24);
                            bmp24_saveImage(image24, "../images/flowers_color_sharpen.bmp"); // Sauvegarde
                        break;
                        case 9:
                            printf("Retourner au menu_precedent :\n");
                        break;
                    }
                    printf("Filtre applique avec succes !\n");
                    printf("Veuillez choisir une option :\n");
                }
            case 4:
                if (im_choix==1) {
                    bmp8_printInfo(image);
                }
                if (im_choix==2) {
                    printf("Informations de l'image 24 bits :\n");
                    printf("Largeur : %d\n", image24->width);
                    printf("Hauteur : %d\n", image24->height);
                    printf("Profondeur de couleur : %d\n", image24->colorDepth);

                }
            break;
            case 5:
                printf("Vous avez quitte le menu");
        }

    }
    // verifie que l'image a bien été chargée
    if (image == NULL) {
        printf("Erreur : Impossible de charger l'image.\n");
        return 1;
    }
/*
    // affiche les informations de l'image
    bmp8_printInfo(image);
    bmp8_saveImage("../images/copie_lena.bmp", image);// Sauvegarde

    // APPLIQUE LE NEGATIF
    bmp8_negative(image);
    bmp8_saveImage("../images/lena_negatif.bmp", image); // Sauvegarde

    // APPLIQUE DE LA LUMINOSITE, EN AJOUTANT +50 PIXEL
    bmp8_brightness(image, 50);
    bmp8_saveImage("../images/lena_bright.bmp", image);// Sauvegarde

    // TRANSFORME L'IMAGE EN UNE IMAGE BINAIRE
    bmp8_threshold(image,128);
    bmp8_saveImage("../images/lena_binary.bmp", image); // Sauvegarde

    //Box  Blur
    image = bmp8_loadImage("../images/lena_gray.bmp");
    float **kernel = createBoxBlurKernel();
    bmp8_applyFilter(image, kernel, 3);
    bmp8_saveImage("../images/lena_boxblur.bmp", image);
    freeKernel(kernel, 3);
    bmp8_free(image);


    //Gaussian Blur
    image = bmp8_loadImage("../images/lena_gray.bmp");
    kernel = createGaussianBlurKernel();
    bmp8_applyFilter(image, kernel, 2);
    bmp8_saveImage("../images/lena_gaussian.bmp", image);
    freeKernel(kernel, 3);
    bmp8_free(image);

    // Outline
    image = bmp8_loadImage("../images/lena_gray.bmp");
    kernel = createOutlineKernel();
    bmp8_applyFilter(image, kernel, 3);
    bmp8_saveImage("../images/lena_outline.bmp", image);
    freeKernel(kernel, 3);
    bmp8_free(image);

    // Emboss
    image = bmp8_loadImage("../images/lena_gray.bmp");
    kernel = createEmbossKernel();
    bmp8_applyFilter(image, kernel, 3);
    bmp8_saveImage("../images/lena_emboss.bmp", image);
    freeKernel(kernel, 3);
    bmp8_free(image);

    // Sharpen
    image = bmp8_loadImage("../images/lena_gray.bmp");
    kernel = createSharpenKernel();
    bmp8_applyFilter(image, kernel, 3);
    bmp8_saveImage("../images/lena_sharpen.bmp", image);
    freeKernel(kernel, 3);
    bmp8_free(image);

    // Partie 2 : Traitement de l'image 24 bits (couleur)
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

    bmp24_saveImage(image24, "../images/copie_flowers_color.bmp"); // Sauvegarde

    // Applique le négatif sur l'image 24 bits
    bmp24_negative(image24);
    bmp24_saveImage(image24, "../images/flowers_color_negatif.bmp"); // Sauvegarde

    // Applique de la luminosité, en ajoutant +50 pixels (image 24 bits)
    bmp24_brightness(image24, 50);
    bmp24_saveImage(image24, "../images/flowers_color_bright.bmp"); // Sauvegarde

    // Transforme l'image en niveaux de gris (image 24 bits)
    bmp24_grayscale(image24);
    bmp24_saveImage(image24, "../images/flowers_color_gray.bmp"); // Sauvegarde




   /// Filtres de convolution ///
    bmp24_boxBlur(image24);
    bmp24_saveImage(image24, "../images/flowers_color_boxblur.bmp");
    bmp24_gaussianBlur(image24);
    bmp24_saveImage(image24, "../images/flowers_color_gaussian.bmp");
    bmp24_outline(image24);
    bmp24_saveImage(image24, "../images/flowers_color_outline.bmp");
    bmp24_emboss(image24);
    bmp24_saveImage(image24, "../images/flowers_color_emboss.bmp");
    bmp24_sharpen(image24);
    bmp24_saveImage(image24, "../images/flowers_color_sharpen.bmp"); // Sauvegarde
    */
    // Libère la mémoire pour l'image 24 bits
    bmp24_free(image24);
    return 0;
}