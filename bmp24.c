#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "bmp24.h"
#include "color_conversion.h"
#include <math.h>

//ça alloue une matrice de pixels pour une image BMP de 24 bits
t_pixel ** bmp24_allocateDataPixels(int width, int height, int colorDepth) {
    //allocation des lignes
    t_pixel **pixels = malloc(height * sizeof(t_pixel *));
    if (pixels == NULL) {
        printf("Erreur d'allocation de la matrice de pixels\n"); //erreur si pas de taille
        return NULL;
    }
    // allocation des colonnes dans les lignes crees précédemment
    for (int i = 0; i < height; i++) {
        pixels[i] = malloc(width * sizeof(t_pixel));
        if (pixels[i] == NULL) {
            printf("Erreur d'allocation des lignes de pixels\n"); //erreur si pas de taille
            return NULL;
        }
    }
    return pixels;//on retourne la matrice de pixel
}

// ça libère la memoire allouée pour la matrice de pixels
void bmp24_freeDataPixels(t_pixel **pixels, int height) {
    for (int i = 0; i < height; i++) {
        free(pixels[i]); // libere  chaque ligne
    }
    free(pixels); //libere le tableau de pointeurs en entier qui est bie, vide maintenant
}

//alloue une structure t_bmp24 pour une image BMP de 24 bits
t_bmp24 * bmp24_allocate(int width, int height, int colorDepth) {
    t_bmp24 *img = malloc(sizeof(t_bmp24)); // on cree le tableau dynamique
    if (img == NULL) {
        printf("Erreur d'allocation de l'image\n");
        return NULL;
    }
    //caracteristiques de l'image :
    img->width = width;
    img->height = height;
    img->colorDepth = colorDepth;
    img->data = bmp24_allocateDataPixels(width, height, colorDepth); // Allocation des données de pixels
    if (img->data == NULL) {
        free(img);//si l'image est vide, on la libere et on return null
        return NULL;
    }
    return img;
}

//libère la mémoire allouée pour une structure t_bmp24
void bmp24_free(t_bmp24 *img) {
    if (img != NULL){
        bmp24_freeDataPixels(img->data, img->height); //on libéree les données avec la fonction faite précédemment
        free(img); //et maintenant que c'est vide, on peut libérer la strcture
    }
}

// lit les données depuis un fichier à une position donnée
void file_rawRead (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file) {
    fseek(file, position, SEEK_SET); //ça se positionne dans le fichier
    fread(buffer, size, n, file); // et maintenant ça lit les données jusqu'à n éléments à lire
}

//ecrit les données dans un fichier à une position donnée
void file_rawWrite (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file) {
    fseek(file, position, SEEK_SET); // ça se positionne dans le fichier
    fwrite(buffer, size, n, file); // ça écrit les données
}

//lit la valeur d'un pixel spécifique dans le fichier et la stocke dans l'image
void bmp24_readPixelValue(t_bmp24 *image, int x, int y, FILE *file) {
    fread(&image->data[y][x].blue, 1, 1, file); // lit le pixel bleu de l'image file
    fread(&image->data[y][x].green, 1, 1, file); //lit le pixel vert de l'image file
    fread(&image->data[y][x].red, 1, 1, file); //lit le pixel rouge de l'image file
}

// lit toutes les données de pixels de l'image depuis le fichier
void bmp24_readPixelData(t_bmp24 *image, FILE *file) {
    fseek(file, image->header.offset, SEEK_SET); //on se positionne au début des données de pixel
    // on lit de bas en haut car c'est un fichier BMP
    for (int y = image->height - 1; y >= 0; y--) {
        for (int x = 0; x < image->width; x++) {
            bmp24_readPixelValue(image, x, y, file); //lit le pixel
        }
    }
}

//ecrit la valeur d'un pixel dans le fichier à partir de l'image
void bmp24_writePixelValue(t_bmp24 *image, int x, int y, FILE *file) {
    fwrite(&image->data[y][x].blue, 1, 1, file); //ecrit le pixel bleu de l'image file
    fwrite(&image->data[y][x].green, 1, 1, file); //ecrit le pixel vert de l'image file
    fwrite(&image->data[y][x].red, 1, 1, file);// ecrit le pixel rouge de l'image file
}

// Écrit toutes les données de pixels de l'image dans le fichier
void bmp24_writePixelData(t_bmp24 *image, FILE *file) {
    fseek(file,image->header.offset,SEEK_SET);//on se positionne au début des données de pixel
    // on écrit là aussi de bas en haut car c'est un fichier BMP
    for (int y = image->height - 1; y >= 0; y--) {
        for (int x = 0; x < image->width; x++) {
            bmp24_writePixelValue(image, x, y, file); //ecrit le pixel
        }
    }
}

//charge l'image BMP de 24 bits
t_bmp24 * bmp24_loadImage(const char *filename) {
    FILE * file = NULL;
    if (filename == NULL) {//onvérifie si le fichier existe bien (si il n'est pas vide)
        printf("Erreur lors de l'ouverture du fichier\n");
        return NULL;
    }

    file = fopen(filename, "rb"); //ouverture du fichier

       // on initialise les caractéristiques de l'image
    int width = 0;
    int height = 0;
    int colorDepth = 0;

    file_rawRead(BITMAP_WIDTH,&width,sizeof(width),1,file); //on lit la largeur
    file_rawRead(BITMAP_DEPTH,&colorDepth,sizeof(colorDepth),1,file); // on lit la profondeur de couleur
    file_rawRead(BITMAP_HEIGHT,&height,sizeof(height),1,file); // on lit la hauteur

    t_bmp24* bmp24 = bmp24_allocate(width, height, colorDepth); // on alloue de l'espace à l'image en fonction de ses caractéristiques
    file_rawRead(BITMAP_MAGIC,&(bmp24->header),sizeof(bmp24->header),1,file); // on lit l'en-tête principal(type, taille..)
    file_rawRead(sizeof(bmp24->header),&(bmp24->header_info),sizeof(bmp24->header_info),1,file); //ça lit les informations de l'en tête(largeur, hauteur, profondeur...)

    bmp24_readPixelData(bmp24,file); // on lit les données des pixels (on fait pas file_rawread parce qu'on parcourt, on ne lit pas un seul bloc)

    fclose(file); //on ferme bien le fichier
    return bmp24;
}

// sauvegarde l'image BMP 24 bits dans un fichier
void bmp24_saveImage(t_bmp24 *img, const char *filename) {
    FILE * file = NULL;
    file = fopen(filename, "wb"); //ouverture du fichier en mode écriture car on la sauvegade
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier\n");
        return;
    }

    // on écrit l'en-tête et les informations d'en-tête dans le fichier
    // donc c'est quasiment le même principe qu'avec le LoadImage
    file_rawWrite(BITMAP_MAGIC,&img->header,sizeof(img->header),1,file);
    file_rawWrite(sizeof(img->header),&img->header_info,sizeof(img->header_info),1,file);
    bmp24_writePixelData(img, file);

    fclose(file);
    printf("Image enregistree avec succes dans '%s'\n", filename);
}

////// FILTRES NEGATIF - GRIS et LUMINOSITE//////

// filtre Négatif
void bmp24_negative(t_bmp24 *img) {
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            img->data[y][x].red = 255 - img->data[y][x].red; //on inverse la couleur rouge de la structure de l'image
            img->data[y][x].green = 255 - img->data[y][x].green; //on inverse la couleur verte de la structure de l'image
            img->data[y][x].blue = 255 - img->data[y][x].blue;//on inverse la couleur bleue de la structure de l'image
        }
    }
}

//filtre gris
void bmp24_grayscale(t_bmp24 *img) {
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            // pour le gris, on va calculer la moyenne des trois couleurs rouge vert et bleue.
            uint8_t gray = (uint8_t)((img->data[y][x].red + img->data[y][x].green + img->data[y][x].blue) / 3);
            img->data[y][x].red = gray;
            img->data[y][x].green = gray;
            img->data[y][x].blue = gray;
        }
    }
}

// filtre de la luminosité
void bmp24_brightness(t_bmp24 *img, int value) {
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            //on fait le canal rouge avec la valeur de saturation qu'a choisi l'utilisateur
            if (img->data[y][x].red + value > 255) {//on vérifie les bornes
                img->data[y][x].red = 255;
            } else {
                img->data[y][x].red = (uint8_t)(img->data[y][x].red + value);
            }

            //on fait le canal vert avec la valeur de saturation qu'a choisi l'utilisateur
            if (img->data[y][x].green + value > 255) {
                img->data[y][x].green = 255;
            } else {
                img->data[y][x].green = (uint8_t)(img->data[y][x].green + value);
            }

            //on fait le canal bleu avec la valeur de saturation qu'a choisi l'utilisateur
            if (img->data[y][x].blue + value > 255) {
                img->data[y][x].blue = 255;
            } else {
                img->data[y][x].blue = (uint8_t)(img->data[y][x].blue + value);
            }
        }
    }
}



///// FILTRES DE CONVOLUTION /////

/// Applique un filtre de convolution à un pixel donné (x, y) dans une image
t_pixel bmp24_convolution(t_bmp24 *img, int x, int y, float **kernel, int kernelSize) {
    int offset = kernelSize / 2; // Décalage pour centrer le noyau autour du pixel
    float red = 0.0, green = 0.0, blue = 0.0; //on prépare les nouvelles valeurs des couleurs

    //on parcourt tous les éléments du noyau
    for (int ky = 0; ky < kernelSize; ky++) {
        for (int kx = 0; kx < kernelSize; kx++) {
            int ix = x + (kx - offset); // Coordonnée x dans l’image
            int iy = y + (ky - offset); // Coordonnée y dans l’image

            //pour ignorer les pixels qui sortent du cadre de l’image
            if (ix < 0 || ix >= img->width || iy < 0 || iy >= img->height)
                continue;

            t_pixel p = img->data[iy][ix];   // Pixel voisin
            float k = kernel[ky][kx];        // Coefficient du noyau

            red += p.red * k; //la nouvelle valeur de la couleur rouge est l'ancienne à laquelle on fait le produit du coefficient du noyau
            green += p.green * k; //la nouvelle valeur de la couleur verte est l'ancienne à laquelle on fait le produit du coefficient du noyau
            blue += p.blue * k; //la nouvelle valeur de la couleur bleue est l'ancienne à laquelle on fait le produit du coefficient du noyau
        }
    }

    // on ne fait pas dépasser 255 ou pas en dessous de 0.
    t_pixel result;
    result.red = (uint8_t)(red< 0 ? 0 : (red > 255 ? 255 : red)); //si red est en dessous de 0, on le met à 0, si au dessus de 255 on met à 255, sinon on met à red (on y touche pas dcp)
    result.green= (uint8_t)(green< 0 ? 0 : (green > 255 ? 255 : green));//si green est en dessous de 0, on le met à 0, si au dessus de 255 on met à 255, sinon on met à green (on y touche pas dcp)
    result.blue = (uint8_t)(blue < 0 ? 0 : (blue > 255 ? 255 : blue));//si blue est en dessous de 0, on le met à 0, si au dessus de 255 on met à 255, sinon on met à blue (on y touche pas dcp)

    return result; //on affiche le nouveau pixel
}

/// Applique un noyau de convolution à toute l’image
void bmp24_applyKernel(t_bmp24 *img, float **kernel, int kernelSize) {
    t_bmp24 *temp = bmp24_allocate(img->width, img->height, img->colorDepth); //image temporaire pour stocker les résultats et pas supprimer les valeurs d'origine

    //on parcourt tous les pixels et on leur applique la convolution
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            temp->data[y][x] = bmp24_convolution(img, x, y, kernel, kernelSize);
        }
    }

    //on reparcourt une fois que c'est fini pour remplacer  les nouveaux pixels dans l'image d'origine
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            img->data[y][x] = temp->data[y][x];
        }
    }

    bmp24_free(temp); //on liibère l’image temporaire
}

///box blur : rend flou l'image de manière égale
void bmp24_boxBlur(t_bmp24 *img) {
    //on crée le kernel
    float **kernel = malloc(3 * sizeof(float *));
    //on remplit le kernel en le parcourant et en le créant de manière dynamique
    for (int i = 0; i < 3; i++) {
        kernel[i] = malloc(3 * sizeof(float));
        for (int j = 0; j < 3; j++) {
            kernel[i][j] = 1.0f / 9.0f; //tous les coefficients sont égaux (1/9 partout)
        }
    }

    bmp24_applyKernel(img, kernel, 3); // Applique le flou

    // Libération de la mémoire du noyau
    for (int i = 0; i < 3; i++) free(kernel[i]);
    free(kernel);
}

///Flou gaussien: rend flou l'image de manière inégale (pas les mêmes coef)
void bmp24_gaussianBlur(t_bmp24 *img) {
  // même principe que précédemment
    float **kernel = malloc(3 * sizeof(float *));
    //sauf que ici les coef ne sont pas pareil donc on crée en dur la matrice
    float values[3][3] = {
        {1/16.0f, 2/16.0f, 1/16.0f},
        {2/16.0f, 4/16.0f, 2/16.0f},
        {1/16.0f, 2/16.0f, 1/16.0f}
    };
    for (int i = 0; i < 3; i++) {
        kernel[i] = malloc(3 * sizeof(float));
        for (int j = 0; j < 3; j++) {
            kernel[i][j] = values[i][j];
        }
    }
    bmp24_applyKernel(img, kernel, 3);
    for (int i = 0; i < 3; i++) free(kernel[i]);
    free(kernel);
}

/// outline : met en évidence les zones avec des changements de couleur
void bmp24_outline(t_bmp24 *img) {
    // même principe que précédemment
    float **kernel = malloc(3 * sizeof(float *));
    float values[3][3] = {
        {-1, -1, -1},
        {-1,  8, -1},
        {-1, -1, -1}
    };
    for (int i = 0; i < 3; i++) {
        kernel[i] = malloc(3 * sizeof(float));
        for (int j = 0; j < 3; j++) {
            kernel[i][j] = values[i][j];
        }
    }

    bmp24_applyKernel(img, kernel, 3); // Met en évidence les bords

    for (int i = 0; i < 3; i++) free(kernel[i]);
    free(kernel);
}

///emboss : donne un effet de relief
void bmp24_emboss(t_bmp24 *img) {
    // même principe que précédemment
    float **kernel = malloc(3 * sizeof(float *));
    float values[3][3] = {
        {-2, -1,  0},
        {-1,  1,  1},
        { 0,  1,  2}
    };
    for (int i = 0; i < 3; i++) {
        kernel[i] = malloc(3 * sizeof(float));
        for (int j = 0; j < 3; j++) {
            kernel[i][j] = values[i][j];
        }
    }

    bmp24_applyKernel(img, kernel, 3); // Applique l'effet de relief

    for (int i = 0; i < 3; i++) free(kernel[i]);
    free(kernel);
}

///sharpen : accentue les détails et les contours
void bmp24_sharpen(t_bmp24 *img) {
    // même principe que précédemment
    float **kernel = malloc(3 * sizeof(float *));
    float values[3][3] = {
        { 0, -1,  0},
        {-1,  5, -1},
        { 0, -1,  0}
    };
    for (int i = 0; i < 3; i++) {
        kernel[i] = malloc(3 * sizeof(float));
        for (int j = 0; j < 3; j++) {
            kernel[i][j] = values[i][j];
        }
    }

    bmp24_applyKernel(img, kernel, 3); // Rend l’image plus nette

    for (int i = 0; i < 3; i++) free(kernel[i]);
    free(kernel);
}



/// Égalisation d'histogramme : améliore le contraste de l'image
void bmp24_equalize(t_bmp24 *img) {
    int width = img->width;
    int height = img->height;

    int hist[256] = {0};     // Histogramme de l’intensité Y (luminance)
    int cdf[256] = {0};      // Fonction de répartition cumulée
    int hist_eq[256] = {0};  // Valeurs Y après égalisation
    int N = width * height;  // Nombre total de pixels

    // Création de matrices pour Y, U, V
    float **Y = malloc(height * sizeof(float *));
    float **U = malloc(height * sizeof(float *));
    float **V = malloc(height * sizeof(float *));
    for (int i = 0; i < height; i++) {
        Y[i] = malloc(width * sizeof(float));
        U[i] = malloc(width * sizeof(float));
        V[i] = malloc(width * sizeof(float));
    }

    // 1. Convertit chaque pixel RGB en YUV et construit l’histogramme des valeurs Y
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            // Convertit RGB -> YUV
            rgb_to_yuv(img->data[i][j].red, img->data[i][j].green, img->data[i][j].blue,
                       &Y[i][j], &U[i][j], &V[i][j]);

            int y_int = round(Y[i][j]); // Arrondit à l’entier le plus proche
            y_int = y_int < 0 ? 0 : (y_int > 255 ? 255 : y_int); // Clamp entre 0 et 255

            hist[y_int]++; // Incrémente le compteur dans l’histogramme
        }
    }

    // 2. Calcule la fonction de répartition cumulée (CDF)
    cdf[0] = hist[0];
    for (int i = 1; i < 256; i++) {
        cdf[i] = cdf[i - 1] + hist[i];
    }

    // 3. Normalise la CDF pour répartir les niveaux d’intensité sur 0–255
    int cdf_min = 0;
    for (int i = 0; i < 256; i++) {
        if (cdf[i] > 0) {
            cdf_min = cdf[i]; // Première valeur non nulle dans la CDF
            break;
        }
    }

    for (int i = 0; i < 256; i++) {
        hist_eq[i] = round(((float)(cdf[i] - cdf_min) / (N - cdf_min)) * 255); // Nouvelle valeur Y normalisée
    }

    // 4. Applique les nouvelles valeurs Y et reconvertit en RGB
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int y_old = round(Y[i][j]);
            y_old = y_old < 0 ? 0 : (y_old > 255 ? 255 : y_old); // Clamp

            Y[i][j] = hist_eq[y_old]; // Remplace Y par sa version égalisée

            uint8_t r, g, b;
            yuv_to_rgb(Y[i][j], U[i][j], V[i][j], &r, &g, &b); // Reconvertit en RGB

            // Mise à jour du pixel avec les nouvelles valeurs
            img->data[i][j].red = r;
            img->data[i][j].green = g;
            img->data[i][j].blue = b;
        }
    }

    // 5. Libère toute la mémoire allouée dynamiquement
    for (int i = 0; i < height; i++) {
        free(Y[i]);
        free(U[i]);
        free(V[i]);
    }
    free(Y);
    free(U);
    free(V);
}
