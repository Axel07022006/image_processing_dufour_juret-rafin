#include <stdio.h>
#include <stdlib.h>


///////////////////////
t_bmp8 * bmp8_loadImage(const char * filename) {
    FILE * file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return NULL;
    }

    // Allouer la structure
    t_bmp8 * image = (t_bmp8 *)malloc(sizeof(t_bmp8));
    if (image == NULL) {
        perror("Erreur d'allocation mémoire");
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
        fprintf(stderr, "Erreur : image non en 8 bits\n");
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
        perror("Erreur d'allocation mémoire pour les données");
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
        perror("Erreur lors de l'ouverture du fichier pour l'écriture");
        return;
    }

    // Écrire l'en-tête BMP (54 octets)
    if (fwrite(img->header, sizeof(unsigned char), 54, file) != 54) {
        perror("Erreur lors de l'écriture de l'en-tête");
        fclose(file);
        return;
    }

    // Écrire la table de couleurs (1024 octets pour image 8 bits)
    if (fwrite(img->colorTable, sizeof(unsigned char), 1024, file) != 1024) {
        perror("Erreur lors de l'écriture de la table de couleurs");
        fclose(file);
        return;
    }

    // Écrire les données de l'image
    if (fwrite(img->data, sizeof(unsigned char), img->dataSize, file) != img->dataSize) {
        perror("Erreur lors de l'écriture des données d'image");
        fclose(file);
        return;
    }

    fclose(file);
    printf("Image enregistrée avec succès dans le dossier Images'%s'\n", filename);
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
    printf("Image Info:\n");
    printf("  Width: %d\n", img->width);
    printf("  Height: %d\n", img->height);
    printf("  Color Depth: %d\n", img->colorDepth);
    printf("  Data Size: %d\n", img->dataSize);
}
///////////////////////

///////////////////////
void bmp8_negative(t_bmp8 * img) {
    if (img == NULL || img->data == NULL) {
        fprintf(stderr, "Erreur : image ou données nulles\n");
        return;
    }

    for (int i = 0; i < img->dataSize; i++) {
        img->data[i] = 255 - img->data[i];
    }
}
///////////////////////

///////////////////////
void bmp8_brightness(t_bmp8 * img, int value) {
    if (img == NULL || img->data == NULL) {
        fprintf(stderr, "Erreur : image ou données nulles\n");
        return;
    }

    for (int i = 0; i < img->dataSize; i++) {
        int newPixel = img->data[i] + value;

        // Clamp entre 0 et 255
        if (newPixel > 255)
            newPixel = 255;
        else if (newPixel < 0)
            newPixel = 0;

        img->data[i] = (unsigned char)newPixel;
    }
}
///////////////////////

///////////////////////
void bmp8_threshold(t_bmp8 * img, int threshold) {
    if (img == NULL || img->data == NULL) {
        fprintf(stderr, "Erreur : image ou données nulles\n");
        return;
    }

    for (int i = 0; i < img->dataSize; i++) {
        if (img->data[i] >= threshold) {
            img->data[i] = 255;
        } else {
            img->data[i] = 0;
        }
    }
}
///////////////////////

///////////////////////
void bmp8_applyFilter(t_bmp8 *img, float **kernel, int kernelSize) {
    if (!img || !img->data) return;

    int width = img->width;
    int height = img->height;
    int n = kernelSize / 2;

    // Création d'un buffer temporaire
    unsigned char *newData = malloc(img->dataSize);
    if (!newData) return;

    // On ne traite pas les bords
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            float sum = 0.0f;

            for (int ky = -n; ky <= n; ky++) {
                for (int kx = -n; kx <= n; kx++) {
                    int pixel = img->data[(y + ky) * width + (x + kx)];
                    float coef = kernel[ky + n][kx + n];
                    sum += coef * pixel;
                }
            }

            // Clamp entre 0 et 255
            if (sum < 0) sum = 0;
            if (sum > 255) sum = 255;

            newData[y * width + x] = (unsigned char)sum;
        }
    }
    // Copie les bords inchangés
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (y == 0 || y == height - 1 || x == 0 || x == width - 1)
                newData[y * width + x] = img->data[y * width + x];
        }
    }
    // Remplace les données d'origine
    free(img->data);
    img->data = newData;
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