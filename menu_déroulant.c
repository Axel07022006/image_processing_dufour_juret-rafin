#include <stdio.h>
#include "bmp8.h"
char chemin[]="images/lena_gray.bmp";

void ouvrir_image(){
  printf("Chemin du fichier : %s\n",chemin);
  printf("Image chargée avec succès !");
  printf("Veuillez choisir une option :");
}

void sauvegarde(){
  printf("Chemin du fichier : %s\n",chemin);
  printf("Image sauvegardée avec succès !");
  printf("Veuillez choisir une option :");
}

void filtre(){
  int choix=0;
  printf("Veuillez choisir un filtre :");
  printf("\t 1. Négatif\n");
  printf("\t 2. Luminosité\n");
  printf("\t 3. Binarisation\n");
  printf("\t 4. Flou\n");
  printf("\t 5. Flou gaussien\n");
  printf("\t 6. Netteté");
  printf("\t 7. Contours");
  printf("\t 8. Relief\n");
  printf("\t 9. Retourner au menu précédent\n");
  printf("Veuillez choisir une option :");
  scanf("%d",&choix);
  printf("Image appliqué avec succès !");
  printf("Veuillez choisir une option :");
}

void info_image() {
  t_bmp8* image = bmp8_loadImage(chemin);  // Charge le fichier
  bmp8_printInfo(image);              // Affiche les infos
}

void menu_déroulant(){
  int menu = 0;

  while (menu != 5) {
    printf("\nVeuillez choisir un menu : \n");
    printf("\t1. Ouvrir une image \n");
    printf("\t2. Sauvegarder une image \n");
    printf("\t3. Appliquer un filtre \n");
    printf("\t4. Afficher les informations de l'image \n");
    printf("\t5. Quitter \n");
    printf("Votre choix : ");
    scanf("%d", &menu);

    switch (menu) {
      case 1:
        ouvrir_image();
      break;
      case 2:
        sauvegarde();
      break;
      case 3:
        filtre();
      break;
      case 4:
        info_image();
      break;
      case 5:
        printf("Fermeture du programme...\n");
      break;
      default:
        printf("Choix invalide. Veuillez réessayer.\n");
    }
  }
}
