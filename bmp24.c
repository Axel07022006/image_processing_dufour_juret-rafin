#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "t_bmp24.h"

// Constantes utiles
#define BITMAP_MAGIC 0x00
#define BITMAP_SIZE 0x02
#define BITMAP_OFFSET 0x0A
#define BITMAP_WIDTH 0x12
#define BITMAP_HEIGHT 0x16
#define BITMAP_DEPTH 0x1C
#define BITMAP_SIZE_RAW 0x22

#define BMP_TYPE 0x4D42
#define HEADER_SIZE 0x0E
#define INFO_SIZE 0x28
#define DEFAULT_DEPTH 0x18 // 24 bits

// Fonction pour allouer dynamiquement une matrice de pixels
t_pixel ** bmp24_allocateDataPixels(int width, int height) {
    t_pixel **pixels = malloc(height * sizeof(t_pixel *));
    if (pixels == NULL) {
        perror("Erreur d'allocation de la matrice de pixels");
        return NULL;
    }

    for (int i = 0; i < height; i++) {
        pixels[i] = malloc(width * sizeof(t_pixel));
        if (pixels[i] == NULL) {
            perror("Erreur d'allocation des lignes de pixels");
            return NULL;
        }
    }

    return pixels;
}

// Fonction pour libérer la matrice de pixels
void bmp24_freeDataPixels(t_pixel **pixels, int height) {
    for (int i = 0; i < height; i++) {
        free(pixels[i]);
    }
    free(pixels);
}

// Fonction pour allouer une image BMP 24 bits
t_bmp24 * bmp24_allocate(int width, int height, int colorDepth) {
    t_bmp24 *img = malloc(sizeof(t_bmp24));
    if (img == NULL) {
        perror("Erreur d'allocation de l'image");
        return NULL;
    }

    img->data = bmp24_allocateDataPixels(width, height);
    if (img->data == NULL) {
        free(img);
        return NULL;
    }

    img->width = width;
    img->height = height;
    img->colorDepth = colorDepth;

    return img;
}

// Fonction pour libérer une image BMP 24 bits
void bmp24_free(t_bmp24 *img) {
    if (img != NULL) {
        bmp24_freeDataPixels(img->data, img->height);
        free(img);
    }
}

// Lecture brute depuis un fichier
void file_rawRead(uint32_t position, void *buffer, uint32_t size, size_t n, FILE *file) {
    fseek(file, position, SEEK_SET);
    fread(buffer, size, n, file);
}

// Écriture brute dans un fichier
void file_rawWrite(uint32_t position, void *buffer, uint32_t size, size_t n, FILE *file) {
    fseek(file, position, SEEK_SET);
    fwrite(buffer, size, n, file);
}

// Fonction pour lire un pixel spécifique
void bmp24_readPixelValue(t_bmp24 *image, int x, int y, FILE *file) {
    uint8_t buffer[3];  // RGB
    file_rawRead(BITMAP_OFFSET + (y * image->width + x) * 3, buffer, sizeof(uint8_t), 3, file);
    image->data[y][x].blue = buffer[0];
    image->data[y][x].green = buffer[1];
    image->data[y][x].red = buffer[2];
}


// Fonction pour lire les données de pixels d'une image BMP 24 bits
void bmp24_readPixelData(t_bmp24 *image, FILE *file) {
    for (int y = image->height - 1; y >= 0; y--) {  // lire de bas en haut
        for (int x = 0; x < image->width; x++) {
            bmp24_readPixelValue(image, x, y, file);
        }
    }
}

// Fonction pour écrire un pixel spécifique
void bmp24_writePixelValue(t_bmp24 *image, int x, int y, FILE *file) {
    uint8_t buffer[3];
    buffer[0] = image->data[y][x].blue;
    buffer[1] = image->data[y][x].green;
    buffer[2] = image->data[y][x].red;

    file_rawWrite(BITMAP_OFFSET + (y * image->width + x) * 3, buffer, sizeof(uint8_t), 3, file);
}

// Fonction pour écrire les données des pixels d'une image BMP 24 bits
void bmp24_writePixelData(t_bmp24 *image, FILE *file) {
    for (int y = image->height - 1; y >= 0; y--) {  // écrire de bas en haut
        for (int x = 0; x < image->width; x++) {
            bmp24_writePixelValue(image, x, y, file);
        }
    }
}


// Fonction pour charger une image BMP 24 bits depuis un fichier
t_bmp24 * bmp24_loadImage(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return NULL;
    }

    t_bmp24 *img = NULL;
    t_bmp_header header;
    t_bmp_info header_info;

    // Lecture des en-têtes
    file_rawRead(BITMAP_MAGIC, &header, sizeof(t_bmp_header), 1, file);
    file_rawRead(BITMAP_OFFSET, &header_info, sizeof(t_bmp_info), 1, file);

    // Vérification du type de fichier BMP
    if (header.type != BMP_TYPE) {
        printf("Ce fichier n'est pas un fichier BMP valide.\n");
        fclose(file);
        return NULL;
    }

    // Allocation de la structure image
    img = bmp24_allocate(header_info.width, header_info.height, 24);
    if (img == NULL) {
        fclose(file);
        return NULL;
    }

    // Lecture des pixels
    bmp24_readPixelData(img, file);

    fclose(file);
    return img;
}

// Fonction pour sauvegarder une image BMP 24 bits dans un fichier
void bmp24_saveImage(t_bmp24 *img, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    // Écriture des en-têtes
    file_rawWrite(BITMAP_MAGIC, &img->header, sizeof(t_bmp_header), 1, file);
    file_rawWrite(BITMAP_OFFSET, &img->header_info, sizeof(t_bmp_info), 1, file);

    // Écriture des pixels
    bmp24_writePixelData(img, file);

    fclose(file);
}


// Fonction pour inverser les couleurs de l'image (Négatif)
void bmp24_negative(t_bmp24 *img) {
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            img->data[y][x].red = 255 - img->data[y][x].red;
            img->data[y][x].green = 255 - img->data[y][x].green;
            img->data[y][x].blue = 255 - img->data[y][x].blue;
        }
    }
}

// Fonction pour convertir l'image en niveaux de gris
void bmp24_grayscale(t_bmp24 *img) {
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            // Calculer la valeur moyenne des trois canaux RGB
            uint8_t gray = (uint8_t)((img->data[y][x].red + img->data[y][x].green + img->data[y][x].blue) / 3);
            img->data[y][x].red = gray;
            img->data[y][x].green = gray;
            img->data[y][x].blue = gray;
        }
    }
}

// Fonction pour ajuster la luminosité de l'image
void bmp24_brightness(t_bmp24 *img, int value) {
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            // Ajouter la valeur à chaque composant de couleur
            img->data[y][x].red = (uint8_t)(img->data[y][x].red + value > 255 ? 255 : img->data[y][x].red + value);
            img->data[y][x].green = (uint8_t)(img->data[y][x].green + value > 255 ? 255 : img->data[y][x].green + value);
            img->data[y][x].blue = (uint8_t)(img->data[y][x].blue + value > 255 ? 255 : img->data[y][x].blue + value);
        }
    }
}