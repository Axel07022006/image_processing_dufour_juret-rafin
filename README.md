
IMAGE PROCESSING EN C


  DESCRIPTION :
  
Ce projet consiste à manipuler des images BMP en 8 et 24 bits en langage C. 
Il a pour objectif de comprendre la structure d'une image, comment la manipuler, la modifier et la représenter. 
Nous pouvons y appliquer différents types de filtre, comme : 
- le Négatif
- la Luminosité
- la Binarisation
Ainsi que les filtres par Convolution qui sont :
- le Flou Gaussien
- le Flou
- la Netteté
- le Contour
- le Relief
Lors de ce projet, nous avons également eu l'occasion d'observer et de calculer l'égalisation des différentes images à travers leur histogramme.
À travers ces différentes options, nous avons mis en place un système de menu afin que l'utilisateur puisse interagir simplement avec notre programme. 


  COMPILATION :

Assurez-vous d’avoir "gcc" installé sur votre machine (ou tout autre compilateur C compatible).
Pour compiler le projet, utilisez la commande suivante dans le terminal :
          "gcc main.c bmp24.c color_conversion.c -o image_tool -lm"


  FONCTIONNALITES IMPLEMENTEES : 

L'utilisateur a la possibilité, à travers un menu dans le terminal, de : 
1. Ouvrir/Charger une des deux images (8bits ou 24bits);
2. De sauvegarder une copie de l'image (sauvegardée dans le dossier image);
3. D'appliquer un filtre à l'image sélectionnée (elle se sauvegardera dans le dossier image lorsque l'utilisateur quittera le programme);
4. De se procurer des informations sur l'image;
5. Pouvoir générer l'image avec son histogramme égalisé;
6. Quitter le programme quand il le souhaite.


Projet Réalisé par :
- DUFOUR Mathias 
- JURET-RAFIN Axel
2025 P1 EFREI PARIS GRP-C









          
