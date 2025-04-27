#ifndef T_BMP24_H
#define T_BMP24_H

#include <stdint.h> // Pour uint8_t, uint16_t, uint32_t, int32_t


// Structure pour représenter un pixel RGB
typedef struct {
    uint8_t red;   // Canal rouge
    uint8_t green; // Canal vert
    uint8_t blue;  // Canal bleu
}t_pixel;

// Structure pour l'en-tête du fichier BMP (14 octets)
typedef struct {
    uint16_t type;       // Signature du fichier ('BM')
    uint32_t size;       // Taille totale du fichier
    uint16_t reserved1;  // Réservé (0)
    uint16_t reserved2;  // Réservé (0)
    uint32_t offset;     // Offset pour les données de l'image
} t_bmp_header;

// Structure pour l'en-tête info du fichier BMP (40 octets)
typedef struct {
    uint32_t size;            // Taille de cette structure
    int32_t width;            // Largeur de l'image
    int32_t height;           // Hauteur de l'image
    uint16_t planes;          // Nombre de plans (1)
    uint16_t bits;            // Bits par pixel (24 ici)
    uint32_t compression;     // Compression (0 = aucune)
    uint32_t imagesize;       // Taille de l'image brute
    int32_t xresolution;      // Résolution horizontale
    int32_t yresolution;      // Résolution verticale
    uint32_t ncolors;         // Nombre de couleurs utilisées
    uint32_t importantcolors; // Nombre de couleurs importantes
} t_bmp_info;

// Structure principale pour une image BMP 24 bits
typedef struct {
    t_bmp_header header;
    t_bmp_info header_info;
    int width;
    int height;
    int colorDepth; // 24 normalement
    t_pixel **data; // Matrice de pixels
} t_bmp24;


#endif // T_BMP24_H