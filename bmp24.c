#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "bmp24.h"



// alloue la matrice de pixels
t_pixel ** bmp24_allocateDataPixels(int width, int height, int colorDepth) {
    t_pixel **pixels = malloc(height * sizeof(t_pixel *));
    if (pixels == NULL) {
        printf("Erreur d'allocation de la matrice de pixels\n");
        return NULL;
    }

    for (int i = 0; i < height; i++) {
        pixels[i] = malloc(width * sizeof(t_pixel));
        if (pixels[i] == NULL) {
            printf("Erreur d'allocation des lignes de pixels\n");
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
        printf("Erreur d'allocation de l'image\n");
        return NULL;
    }

    img->width = width;
    img->height = height;
    img->colorDepth = colorDepth;
    img->data = bmp24_allocateDataPixels(width, height, colorDepth);
    if (img->data == NULL) {
        free(img);
        return NULL;
    }

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
    fread(&image->data[y][x].blue,1,1,file);
    fread(&image->data[y][x].green,1,1,file);
    fread(&image->data[y][x].red,1,1,file);
}


// lit les données des pixels de l'image
void bmp24_readPixelData(t_bmp24 *image, FILE *file) {
    fseek(file, image->header.offset, SEEK_SET);
    for (int y = image->height - 1; y >= 0; y--) {  // lire de bas en haut
        for (int x = 0; x < image->width; x++) {
            bmp24_readPixelValue(image, x, y, file);
        }
    }
}


// écrit un pixel specifique
void bmp24_writePixelValue(t_bmp24 *image, int x, int y, FILE *file) {
    fwrite(&image->data[y][x].blue,1,1,file);
    fwrite(&image->data[y][x].green,1,1,file);
    fwrite(&image->data[y][x].red,1,1,file);
}

// ecrit les donnees d'un pixel d'une image
void bmp24_writePixelData(t_bmp24 *image, FILE *file) {
    fseek(file,image->header.offset,SEEK_SET);
    for (int y = image->height - 1; y >= 0; y--) {  // écrire de bas en haut
        for (int x = 0; x < image->width; x++) {
            bmp24_writePixelValue(image, x, y, file);
        }
    }
}


// charge l'image depuis un fichier
t_bmp24 * bmp24_loadImage(const char *filename) {
    FILE * file = NULL;
    if (filename == NULL) {
        printf("Erreur lors de l'ouverture du fichier\n");
        return NULL;
    }

    file = fopen(filename, "rb");

    int width,height,colorDepth;
    file_rawRead(BITMAP_WIDTH,&width,sizeof(width),1,file);
    file_rawRead(BITMAP_HEIGHT,&height,sizeof(height),1,file);
    file_rawRead(BITMAP_DEPTH,&colorDepth,sizeof(colorDepth),1,file);

    t_bmp24* bmp24 = bmp24_allocate(width, height, colorDepth);
    file_rawRead(BITMAP_MAGIC,&(bmp24->header),sizeof(bmp24->header),1,file);
    file_rawRead(sizeof(bmp24->header),&(bmp24->header_info),sizeof(bmp24->header_info),1,file);
    bmp24_readPixelData(bmp24,file);

    fclose(file);
    return bmp24;
}

// sauvegarde l'image
void bmp24_saveImage(t_bmp24 *img, const char *filename) {
    FILE * file = NULL;
    file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier\n");
        return;
    }


    // Écriture dans le fichier
    file_rawWrite(BITMAP_MAGIC,&img->header,sizeof(img->header),1,file);
    file_rawWrite(sizeof(img->header),&img->header_info,sizeof(img->header_info),1,file);
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
t_pixel bmp24_convolution (t_bmp24 * img, int x, int y, float ** kernel, int kernelSize){t_pixel p; return p;}
void bmp24_applyKernel(t_bmp24 *img, float **kernel, int kernelSize){}

void bmp24_boxBlur(t_bmp24 *img){}
void bmp24_gaussianBlur(t_bmp24 *img){}
void bmp24_outline(t_bmp24 *img){}
void bmp24_emboss(t_bmp24 *img){}
void bmp24_sharpen(t_bmp24 *img){}