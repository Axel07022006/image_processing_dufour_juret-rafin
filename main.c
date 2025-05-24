#include <stdio.h>
#include <stdlib.h>
#include "bmp8.h"
#include "bmp24.h"
#include "color_conversion.h"

int main() {
    // Charge l'image BMP en niveaux de gris
    t_bmp8 * image = bmp8_loadImage("../images/barbara_gray.bmp");
    t_bmp24 *image24 = bmp24_loadImage("../images/flowers_color.bmp");
    //t_bmp8 * image = bmp8_loadImage("barbara_gray.bmp");
    if (image == NULL || image24 == NULL) {
        printf("testErreur : Impossible de charger l'image.\n");
        return 1;
    }
    int im_choix=0;
    while (im_choix==0) {
        printf("Choisissez une image parmi les deux suivantes : \n");
        printf("1. Image de barbara_gray\n");
        printf("2. Images de fleurs\n");
        printf("Vous choisissez : ");
        scanf("%d",&im_choix);
    }
    int choix=7;
    while (choix!=6 ) {
        printf("Veuillez choisir une option :\n");
        printf("\t1. Ouvrir une image\n");
        printf("\t2. Sauvegarder une image\n");
        printf("\t3. Appliquer un filtre\n");
        printf("\t4. Afficher les informations de l'image\n");
        printf("\t5. Generer l'image egalise\n");
        printf("\t6. Quitter\n");
        printf("Choisissez parmi ces choix : ");
        scanf("%d",&choix);
        printf("Votre choix : %d \n\n",choix);
        switch (choix) {
            case 1:
                if (im_choix==1) {
                    char chemin_barbara[]="../images/barbara_gray.bmp";
                    printf("Chemin du fichier : %s \n",chemin_barbara);
                    image = bmp8_loadImage("../images/barbara_gray.bmp");
                    printf("Image chargee avec succes ! \n");
                }
                if (im_choix==2) {
                    char chemin_flowers[]="../images/flowers_color.bmp";
                    printf("Chemin du fichier :%s \n",chemin_flowers);
                    image24 = bmp24_loadImage("../images/flowers_color.bmp");
                    printf("Image chargee avec succes ! \n");
                }
            break;
            case 2:
                if (im_choix==1) {
                    bmp8_saveImage("../images/copie_barbara.bmp", image);
                    printf("Image sauvagardee avec succes ! \n");
                    printf("Veuillez choisir une option :\n");
                }
                if (im_choix==2) {
                    bmp24_saveImage(image24, "../images/copie_flowers_color.bmp"); // Sauvegarde
                    bmp24_free(image24); // Libere immediatement
                    printf("Image sauvagardee avec succes ! \n");
                    printf("Veuillez choisir une option :\n");
                }

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
                            bmp8_saveImage("../images/barbara_negatif.bmp", image);
                            printf("Filtre applique avec succes ! \n\n");
                        break;
                        case 2:
                            bmp8_brightness(image, 50);
                            bmp8_saveImage("../images/barbara_bright.bmp", image);
                            printf("Filtre applique avec succes ! \n\n");
                        break;
                        case 3:
                            // TRANSFORME L'IMAGE EN UNE IMAGE BINAIRE
                            bmp8_threshold(image,128);
                            bmp8_saveImage("../images/barbara_binary.bmp", image); // Sauvegarde
                            printf("Filtre applique avec succes ! \n\n");
                        break;
                        case 4:
                            image = bmp8_loadImage("../images/barbara_gray.bmp");
                            float **kernel = createBoxBlurKernel();
                            bmp8_applyFilter(image, kernel, 3);
                            bmp8_saveImage("../images/barbara_boxblur.bmp", image);
                            freeKernel(kernel, 3);
                            bmp8_free(image);
                            printf("Filtre applique avec succes ! \n\n");
                        break;
                        case 5:
                            //Gaussian Blur
                            image = bmp8_loadImage("../images/barbara_gray.bmp");
                            kernel = createGaussianBlurKernel();
                            bmp8_applyFilter(image, kernel, 2);
                            bmp8_saveImage("../images/barbara_gaussian.bmp", image);
                            freeKernel(kernel, 3);
                            bmp8_free(image);
                            printf("Filtre applique avec succes ! \n\n");
                        break;
                        case 6:
                            // Emboss
                            image = bmp8_loadImage("../images/barbara_gray.bmp");
                            kernel = createEmbossKernel();
                            bmp8_applyFilter(image, kernel, 3);
                            bmp8_saveImage("../images/barbara_emboss.bmp", image);
                            freeKernel(kernel, 3);
                            bmp8_free(image);
                            printf("Filtre applique avec succes ! \n\n");
                        break;
                        case 7:
                            // Outline
                            image = bmp8_loadImage("../images/barbara_gray.bmp");
                            kernel = createOutlineKernel();
                            bmp8_applyFilter(image, kernel, 3);
                            bmp8_saveImage("../images/barbara_outline.bmp", image);
                            freeKernel(kernel, 3);
                            bmp8_free(image);
                            printf("Filtre applique avec succes ! \n\n");
                        break;
                        case 8:
                            // Sharpen
                            image = bmp8_loadImage("../images/barbara_gray.bmp");
                            kernel = createSharpenKernel();
                            bmp8_applyFilter(image, kernel, 3);
                            bmp8_saveImage("../images/barbara_sharpen.bmp", image);
                            freeKernel(kernel, 3);
                            bmp8_free(image);
                            printf("Filtre applique avec succes ! \n\n");
                        break;
                        case 9:
                            printf("Retourner au menu_precedent :\n");
                        break;
                    }

                    printf("Veuillez choisir une option :\n");
                }
                if (im_choix==2) {
                    switch (filtre_choix) {
                        case 1:
                            image24 = bmp24_loadImage("../images/flowers_color.bmp");
                            bmp24_negative(image24);
                            bmp24_saveImage(image24, "../images/flowers_color_negatif.bmp");
                            bmp24_free(image24);
                            printf("Filtre applique avec succes ! \n\n");
                        break;
                        case 2:
                            image24 = bmp24_loadImage("../images/flowers_color.bmp");
                            bmp24_brightness(image24, 50);
                            bmp24_saveImage(image24, "../images/flowers_color_bright.bmp");
                            bmp24_free(image24);
                            printf("Filtre applique avec succes ! \n\n");
                        break;
                        case 3:
                            image24 = bmp24_loadImage("../images/flowers_color.bmp");
                            bmp24_grayscale(image24);
                            bmp24_saveImage(image24, "../images/flowers_color_gray.bmp");
                            bmp24_free(image24);
                            printf("Filtre applique avec succes ! \n\n");
                        break;
                        case 4:
                            image24 = bmp24_loadImage("../images/flowers_color.bmp");
                            bmp24_boxBlur(image24);
                            bmp24_saveImage(image24, "../images/flowers_color_boxblur.bmp");
                            bmp24_free(image24);
                            printf("Filtre applique avec succes ! \n\n");
                        break;
                        case 5:
                            image24 = bmp24_loadImage("../images/flowers_color.bmp");
                            bmp24_gaussianBlur(image24);
                            bmp24_saveImage(image24, "../images/flowers_color_gaussian.bmp");
                            bmp24_free(image24);
                            printf("Filtre applique avec succes ! \n\n");
                        break;
                        case 6:
                            // EMBOSS
                            image24 = bmp24_loadImage("../images/flowers_color.bmp");
                            bmp24_emboss(image24);
                            bmp24_saveImage(image24, "../images/flowers_color_emboss.bmp");
                            bmp24_free(image24);
                            printf("Filtre applique avec succes ! \n\n");

                        break;
                        case 7:
                            // OUTLINE
                            image24 = bmp24_loadImage("../images/flowers_color.bmp");
                            bmp24_outline(image24);
                            bmp24_saveImage(image24, "../images/flowers_color_outline.bmp");
                            bmp24_free(image24);
                            printf("Filtre applique avec succes ! \n\n");
                        break;
                        case 8:
                            // SHARPEN
                            image24 = bmp24_loadImage("../images/flowers_color.bmp");
                            bmp24_sharpen(image24);
                            bmp24_saveImage(image24, "../images/flowers_color_sharpen.bmp");
                            bmp24_free(image24);
                            printf("Filtre applique avec succes ! \n\n");

                        break;
                        case 9:
                            printf("Retourner au menu_precedent :\n");
                        break;
                    }
                    printf("Veuillez choisir une option :\n");
                }
            break;
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
                if (im_choix==1) {
                    if (image) {
                        unsigned int *hist = bmp8_computeHistogram(image);
                        unsigned int *cdf = bmp8_computeCDF(hist);
                        bmp8_equalize(image, cdf);
                        bmp8_saveImage("../images/barbara_gray_equalized.bmp", image);
                        printf("L'image egalisee a bien ete genere !\n");
                        bmp8_free(image);
                        free(hist);
                        free(cdf);
                    }
                }if (im_choix==2) {
                    if (image24) {
                        bmp24_equalize(image24);
                        bmp24_saveImage(image24, "../images/flowers_equalized.bmp");
                        printf("L'image egalisee a bien ete genere !\n");
                        bmp24_free(image24);
                    }
                }
            break;
            case 6:
                printf("\nVous avez quitte le menu\n");
            break;
        }

    }

    return 0;
}