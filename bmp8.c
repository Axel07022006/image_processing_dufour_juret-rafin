#include <stdio.h>
#include <stdlib.h>
#include "bmp8.h"


///////////////////////
t_bmp8 * bmp8_loadImage(const char * filename) {

    FILE * file = fopen(filename, "rb");
    if (file == NULL) {
        printf("file == NULL\n");
        printf("Erreur lors de l'ouverture du fichier\n");
        return NULL;
    }

    // Allouer la structure
    t_bmp8 * image = (t_bmp8 *)malloc(sizeof(t_bmp8));
    if (image == NULL) {
        printf("Erreur d'allocation mémoire\n");
        fclose(file);
        return NULL;
    }

    // Lire l'en-tête (54 octets)
    fread(image->header, sizeof(unsigned char), 54, file);

    // Extraire les champs depuis l'en-tête BMP
    image->width = *(unsigned int *)(&image->header[18]);
    image->height = *(unsigned int *)(&image->header[22]);
    image->colorDepth = *(unsigned int *)(&image->header[28]);

    // Vérification que l'image est bien en 8 bits
    if (image->colorDepth != 8) {
        printf("Erreur : image non en 8 bits\n");
        fclose(file);
        free(image);
        return NULL;
    }

    // Lire la table de couleur (1024 octets)
    fread(image->colorTable, sizeof(unsigned char), 1024, file);

    // Calcul de la taille des données d'image
    image->dataSize = image->width * image->height;

    // Allouer la mémoire pour les données
    image->data = (unsigned char *)malloc(image->dataSize * sizeof(unsigned char));
    if (image->data == NULL) {
        printf("Erreur d'allocation mémoire pour les données\n");
        fclose(file);
        free(image);
        return NULL;
    }

    // Lire les données
    fread(image->data, sizeof(unsigned char), image->dataSize, file);

    fclose(file);
    return image;
}
///////////////////////

///////////////////////
void bmp8_saveImage(const char * filename, t_bmp8 * img) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier pour l'ecriture\n");
        return;
    }

    // Écrire l'en-tête BMP (54 octets)
    if (fwrite(img->header, sizeof(unsigned char), 54, file) != 54) {
        printf("Erreur lors de l'ecriture de l'en-tete\n");
        fclose(file);
        return;
    }

    // Écrire la table de couleurs (1024 octets pour image 8 bits)
    if (fwrite(img->colorTable, sizeof(unsigned char), 1024, file) != 1024) {
        printf("Erreur lors de l'ecriture de la table de couleurs\n");
        fclose(file);
        return;
    }

    // Écrire les données de l'image
    if (fwrite(img->data, sizeof(unsigned char), img->dataSize, file) != img->dataSize) {
        printf("Erreur lors de l'ecriture des données d'image\n");
        fclose(file);
        return;
    }

    fclose(file);
    printf("Image enregistree avec succes dans le dossier Images'%s'\n", filename);
}
///////////////////////

///////////////////////
void bmp8_free(t_bmp8 * img) {
    if (img != NULL) {
        free(img->data);
        free(img);
    }
}
///////////////////////

///////////////////////
void bmp8_printInfo(t_bmp8 * img){
    printf("Image Info 8bits:\n");
    printf("  Width: %d\n", img->width);
    printf("  Height: %d\n", img->height);
    printf("  Color Depth: %d\n", img->colorDepth);
    printf("  Data Size: %d\n", img->dataSize);
}
///////////////////////

///////////////////////
void bmp8_negative(t_bmp8 * img) {
    if (img == NULL || img->data == NULL) {
        printf("Erreur : image ou données nulles\n");
        return;
    }

    // Parcours de tous les pixels de l'image
    for (int i = 0; i < img->dataSize; i++) {
        // Inversion du niveau de gris : plus clair devient sombre et inversement
        img->data[i] = 255 - img->data[i];
    }

    // À la fin de la fonction, l'image est transformée en négatif
}

///////////////////////

///////////////////////
void bmp8_brightness(t_bmp8 * img, int value) {
    if (img == NULL || img->data == NULL) {
        printf("Erreur : image ou données nulles\n");
        return;
    }

    // Pour chaque pixel, on ajuste la luminosité
    for (int i = 0; i < img->dataSize; i++) {
        int newPixel = img->data[i] + value; // Ajoute la valeur donnée

        // oblige bien que que newPixel est contenu entre 0 et 255
        if (newPixel > 255)
            newPixel = 255;
        else if (newPixel < 0)
            newPixel = 0;

        // Affecte la nouvelle valeur au pixel
        img->data[i] = (unsigned char)newPixel;
    }

    // À la fin : l'image est plus claire ou plus sombre selon value
}

///////////////////////

///////////////////////
void bmp8_threshold(t_bmp8 * img, int threshold) {
    if (img == NULL || img->data == NULL) {
        printf("Erreur : image ou données nulles\n");
        return;
    }

    // Parcours de tous les pixels de l’image
    for (int i = 0; i < img->dataSize; i++) {
        // Si la valeur du pixel est supérieure ou égale au seuil : blanc
        // Sinon : noir
        if (img->data[i] >= threshold) {
            img->data[i] = 255;
        } else {
            img->data[i] = 0;
        }
    }

    // L’image devient binaire (noir/blanc) selon le seuil défini
}

///////////////////////

///////////////////////
void bmp8_applyFilter(t_bmp8 *img, float **kernel, int kernelSize) {
    if (!img || !img->data) return;

    int width = img->width;
    int height = img->height;
    int n = kernelSize / 2; // Centre du noyau

    unsigned char *newData = malloc(img->dataSize);
    if (!newData) return;

    // Parcours de chaque pixel
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            float sum = 0.0f;

            // Application du noyau de convolution centré sur (x, y)
            for (int ky = -n; ky <= n; ky++) {
                for (int kx = -n; kx <= n; kx++) {
                    int pixel = img->data[(y + ky) * width + (x + kx)];
                    float coef = kernel[ky + n][kx + n];
                    sum += coef * pixel; // Contribution pondérée du pixel voisin
                }
            }

            // s'assure que la somme est dans l'intervalle [0, 255]
            if (sum < 0) sum = 0;
            if (sum > 255) sum = 255;

            newData[y * width + x] = (unsigned char)sum;
        }
    }

    // Copie des bords sans modification
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (y == 0 || y == height - 1 || x == 0 || x == width - 1)
                newData[y * width + x] = img->data[y * width + x];
        }
    }

    // Remplacement des anciennes données
    free(img->data);
    img->data = newData;

    // À la fin, l'image contient l'effet du filtre appliqué
}

float **createKernel(float values[3][3], float divisor) {
    float **kernel = malloc(3 * sizeof(float *));
    for (int i = 0; i < 3; i++) {
        kernel[i] = malloc(3 * sizeof(float));
        for (int j = 0; j < 3; j++) {
            kernel[i][j] = values[i][j] / divisor;
        }
    }
    return kernel;
}

void freeKernel(float **kernel, int size) {
    for (int i = 0; i < size; i++) {
        free(kernel[i]);
    }
    free(kernel);
}

// LES 5 NOYAUX
float **createBoxBlurKernel() {
    float values[3][3] = {
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    };
    return createKernel(values, 9.0f);
}

float **createGaussianBlurKernel() {
    float values[3][3] = {
        {1, 2, 1},
        {2, 4, 2},
        {1, 2, 1}
    };
    return createKernel(values, 16.0f);
}

float **createOutlineKernel() {
    float values[3][3] = {
        {-1, -1, -1},
        {-1,  8, -1},
        {-1, -1, -1}
    };
    return createKernel(values, 1.0f);
}

float **createEmbossKernel() {
    float values[3][3] = {
        {-2, -1, 0},
        {-1,  1, 1},
        { 0,  1, 2}
    };
    return createKernel(values, 1.0f);
}

float **createSharpenKernel() {
    float values[3][3] = {
        { 0, -1,  0},
        {-1,  5, -1},
        { 0, -1,  0}
    };
    return createKernel(values, 1.0f);
}
///////////////////////
///

#include <math.h>


unsigned int * bmp8_computeHistogram(t_bmp8 * img) {
    // Alloue un tableau de 256 entiers, initialisés à 0
    unsigned int *hist = (unsigned int *)calloc(256, sizeof(unsigned int));
    if (!hist) {
        fprintf(stderr, "Erreur d'allocation pour l'histogramme.\n");
        return NULL;
    }

    // Compte combien de fois chaque niveau de gris apparaît
    for (unsigned int i = 0; i < img->dataSize; i++) {
        hist[img->data[i]]++; // Incrémente le compteur pour la valeur du pixel
    }

    return hist; // Retourne l’histogramme
}


unsigned int * bmp8_computeCDF(unsigned int * hist) {
    // Alloue un tableau pour l'histogramme égalisé
    unsigned int *hist_eq = (unsigned int *)calloc(256, sizeof(unsigned int));
    if (!hist_eq) {
        fprintf(stderr, "Erreur d'allocation pour la CDF.\n");
        return NULL;
    }

    unsigned int cdf[256] = {0};   // CDF non normalisée
    unsigned int cdfmin = 0;       // Premier élément non nul (pour normaliser)
    unsigned int total_pixels = 0;

    // Calcul de la CDF (somme cumulative de l’histogramme)
    for (int i = 0; i < 256; i++) {
        total_pixels += hist[i];
        cdf[i] = (i == 0) ? hist[i] : cdf[i - 1] + hist[i];

        // On cherche le plus petit CDF non nul (nécessaire à la normalisation)
        if (cdfmin == 0 && cdf[i] != 0) {
            cdfmin = cdf[i];
        }
    }

    // Normalisation du CDF pour obtenir des niveaux de gris sur [0, 255]
    for (int i = 0; i < 256; i++) {
        hist_eq[i] = round(((double)(cdf[i] - cdfmin) / (total_pixels - cdfmin)) * 255);
    }

    return hist_eq; // Renvoie la table de conversion (mapping)
}


void bmp8_equalize(t_bmp8 * img, unsigned int * hist_eq) {
    if (!img || !img->data || !hist_eq) {
        fprintf(stderr, "Erreur : image ou histogramme egalise invalide.\n");
        return;
    }

    // Applique la transformation à chaque pixel
    for (unsigned int i = 0; i < img->dataSize; i++) {
        // L’ancien niveau de gris devient le nouveau (selon la table d’égalisation)
        img->data[i] = (unsigned char)hist_eq[img->data[i]];
    }

    // L’image a maintenant un contraste étendu (plus uniforme)
}


