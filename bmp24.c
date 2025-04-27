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

// alloue la matrice de pixels
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

// libère la mémoire allouée
void bmp24_freeDataPixels(t_pixel **pixels, int height) {
    for (int i = 0; i < height; i++) {
        free(pixels[i]);
    }
    free(pixels);
}

// alloue l'image de 24 bits
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

// libère la mémoire allouée
void bmp24_free(t_bmp24 *img) {
    if (img != NULL) {
        bmp24_freeDataPixels(img->data, img->height);
        free(img);
    }
}


//lit depuis un fichier
void file_rawRead (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file) {
    fseek(file, position, SEEK_SET);
    fread(buffer, size, n, file);
}

// écrit dans un fichier
void file_rawWrite (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file) {
    fseek(file, position, SEEK_SET);
    fwrite(buffer, size, n, file);
}


// lit un pixel spécifique
void bmp24_readPixelValue(t_bmp24 *image, int x, int y, FILE *file) {
    uint8_t buffer[3];  //
    t_bmp_header header;
    file_rawRead(BITMAP_MAGIC, &header, sizeof(t_bmp_header), 1, file);
    image->data[y][x].blue = buffer[0];
    image->data[y][x].green = buffer[1];
    image->data[y][x].red = buffer[2];
}

// lit les données des pixels de l'image
void bmp24_readPixelData(t_bmp24 *image, FILE *file) {
    for (int y = image->height - 1; y >= 0; y--) {  // lire de bas en haut
        for (int x = 0; x < image->width; x++) {
            bmp24_readPixelValue(image, x, y, file);
        }
    }
}


// écrit un pixel specifique
void bmp24_writePixelValue(t_bmp24 *image, int x, int y, FILE *file) {
    uint8_t buffer[3];
    buffer[0] = image->data[y][x].blue;
    buffer[1] = image->data[y][x].green;
    buffer[2] = image->data[y][x].red;

    file_rawWrite(BITMAP_OFFSET + (y * image->width + x) * 3, buffer, sizeof(uint8_t), 3, file);
}

// ecrit les donnees d'un pixel d'une image
void bmp24_writePixelData(t_bmp24 *image, FILE *file) {
    for (int y = image->height - 1; y >= 0; y--) {  // écrire de bas en haut
        for (int x = 0; x < image->width; x++) {
            bmp24_writePixelValue(image, x, y, file);
        }
    }
}


// charge l'image depuis un fichier
t_bmp24 * bmp24_loadImage(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return NULL;
    }

    t_bmp24 *img = NULL;
    t_bmp_header header;
    t_bmp_info header_info;

    // lecture des en-têtes
    file_rawRead(BITMAP_MAGIC, &header, sizeof(t_bmp_header), 1, file);
    file_rawRead(BITMAP_OFFSET, &header_info, sizeof(t_bmp_info), 1, file);

    //vérification du type de fichier
    if (header.type != BMP_TYPE) {
        printf("Ce fichier n'est pas un fichier BMP.\n");
        fclose(file);
        return NULL;
    }

    //  cela alloue l'image et remplit ses informations
    img = bmp24_allocate(header_info.width, header_info.height, 24);
    if (img == NULL) {
        fclose(file);
        return NULL;
    }

    img->header_info.xresolution = header_info.xresolution;  // Résolution horizontale
    img->header_info.yresolution = header_info.yresolution;  // Résolution verticale
    img->header_info.ncolors = header_info.ncolors;          // Nombre de couleurs
    img->header_info.importantcolors = header_info.importantcolors;  // Nombre de couleurs importantes

    // lit les pixels
    bmp24_readPixelData(img, file);

    fclose(file);
    return img;
}

// sauvegarde l'image
void bmp24_saveImage(t_bmp24 *img, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    // définit la taille de l'image
    img->header.size = sizeof(t_bmp_header) + sizeof(t_bmp_info) + (img->width * img->height * 3);
    img->header.offset = sizeof(t_bmp_header) + sizeof(t_bmp_info);

    //initialise les valeurs par défaut pour les champs non-utilisés
    img->header_info.xresolution = 0x0B13;  // 72 DPI
    img->header_info.yresolution = 0x0B13;
    img->header_info.ncolors = 0;           // Pas de palette
    img->header_info.importantcolors = 0;   // Pas de couleurs importantes

    // écrit l'entête
    fwrite(&img->header, sizeof(t_bmp_header), 1, file);

    // écrit le header
    fwrite(&img->header_info, sizeof(t_bmp_info), 1, file);

    // écrit les pixels
    bmp24_writePixelData(img, file);

    fclose(file);
    printf("Image enregistrée avec succès dans '%s'\n", filename);
}


// foncition negative -- A REVOIR
void bmp24_negative(t_bmp24 *img) {
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            img->data[y][x].red = 255 - img->data[y][x].red;
            img->data[y][x].green = 255 - img->data[y][x].green;
            img->data[y][x].blue = 255 - img->data[y][x].blue;
        }
    }
}

//fonction niveaux de gris --- A REVOIR
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

// fonction luminosite- --- A REVOIR
void bmp24_brightness(t_bmp24 *img, int value) {
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            // Ajouter la valeur à chaque composant de couleur
            if (img->data[y][x].red + value > 255) {
                img->data[y][x].red = 255;
            } else {
                img->data[y][x].red = (uint8_t)(img->data[y][x].red + value);
            }

            if (img->data[y][x].green + value > 255) {
                img->data[y][x].green = 255;
            } else {
                img->data[y][x].green = (uint8_t)(img->data[y][x].green + value);
            }

            if (img->data[y][x].blue + value > 255) {
                img->data[y][x].blue = 255;
            } else {
                img->data[y][x].blue = (uint8_t)(img->data[y][x].blue + value);
            }
        }
    }
}





/// Filtres de convolution -- A FAIRE ----------
///
t_pixel bmp24_convolution (t_bmp24 * img, int x, int y, float ** kernel, int kernelSize);
void bmp24_applyKernel(t_bmp24 *img, float **kernel, int kernelSize);

void bmp24_boxBlur(t_bmp24 *img);
void bmp24_gaussianBlur(t_bmp24 *img);
void bmp24_outline(t_bmp24 *img);
void bmp24_emboss(t_bmp24 *img);
void bmp24_sharpen(t_bmp24 *img);