#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include "limace.h"
#include "tai.h"

#define DEBOGAGE

#ifdef DEBOGAGE
#define DEBUG fprintf(stderr,"Fichier %s, ligne %d\n",__FILE__,__LINE__);
#else
#define DEBUG
#endif

#define AFAIRE(fonction) fprintf(stderr,"Corps de la fonction " #fonction \
                                        " à écrire.\n" ); \
												 return NULL;
#define AFAIRE2(fonction) fprintf(stderr,"Corps de la fonction " #fonction \
                                        " à écrire.\n" ); \
												 return 0;

/*
 * Conversion d'une image couleur en une image de niveaux de gris
 * Entrée : image initiale en couleur
 * Sortie : image de niveaux de gris résultat
 */
Image RGB2Gray(Image Im)
{
  unsigned char ** R = ImGetR(Im);
  unsigned char ** G = ImGetG(Im);
  unsigned char ** B = ImGetB(Im);
  Image ImI;
  int nbRow = ImNbRow(Im);
  int nbCol = ImNbCol(Im);
  ImI = ImAlloc(GrayLevel, nbRow, nbCol);
  unsigned char ** I = ImGetI(ImI);
  for(int i = 0; i < nbRow; i++){
    for(int j = 0; j < nbCol; j++){
      I[i][j] = round(0.299 * R[i][j] + 0.587 * G[i][j] + 0.114 * B[i][j]);
    }
  }
  return ImI;
}


/*
 * Binarisation d'une image de niveaux de gris par seuillage global
 * Entrées : image de niveaux de gris initiale
             seuil (niveau de gris)
 * Sortie : image binaire
 */
Image Binarization(Image Im, unsigned char Threshold)
{
  int nbRow = ImNbRow(Im);
  int nbCol = ImNbCol(Im);
  Image ImI = ImAlloc(BitMap, nbRow, nbCol);
  unsigned char ** I = ImGetI(Im);
  unsigned char ** I2 = ImGetI(ImI);
  for(int i = 0; i < nbRow; i++){
    for(int j = 0; j < nbCol; j++){
      if ( I[i][j]< Threshold){
        I2[i][j]=0;
      }
      else{
        I2[i][j]=1;
      }
    }
  }
  return  ImI;
}


/*
 * Inversion d'une image
 * Entrée : image initiale (binaire, niveaux de gris ou couleur)
 * Sortie : image résultat
 */
Image Inversion(Image Im)
{
  Image ImNew;
  int nbRow = ImNbRow(Im); int nbCol = ImNbCol(Im);
  if(ImType(Im) == Color){
    unsigned char ** R1, ** G1, ** B1, ** R2, ** G2, ** B2;
    R1 = ImGetR(Im); G1 = ImGetG(Im); B1 = ImGetB(Im);
    R2 = ImGetR(Im); G2 = ImGetG(Im); B2 = ImGetB(Im);
    for(int i = 0; i < nbRow; i++){
      for(int j = 0; j < nbCol; j++){
        R2[i][j] = 255 - R1[i][j];
        G2[i][j] = 255 - G1[i][j];
        B2[i][j] = 255 - B1[i][j];
      }
    }
  }else if (ImType(Im) == GrayLevel){
    unsigned char ** I1, ** I2;
    I1 = ImGetI(Im);
    I2 = ImGetI(ImNew);
    for(int i = 0; i < nbRow; i++){
      for(int j = 0; j < nbCol; j++){
        I2[i][j] = 255 - I1[i][j];
      }
    }
  }
  else{
    unsigned char ** I1, ** I2;
    I1 = ImGetI(Im);
    I2 = ImGetI(ImNew);
    for(int i = 0; i < nbRow; i++){
      for(int j = 0; j < nbCol; j++){
        I2[i][j] = (1+I1[i][j])%2;
      }
    }
  }
  return ImNew;
}


/*
 * Calcul de l'histogramme d'une image de niveaux de gris
 * Entrée : image initiale (niveaux de gris)
 * Sortie : histogramme (matrice de int 1 x 256)
 */
 Matrix Histogram(Image Im)
 {
   Matrix hist = MatCAlloc(Int,1,256);
   int nbRow = ImNbRow(Im); int nbCol = ImNbCol(Im);
   unsigned char **Gris = ImGetI(Im);
   int gris;
   int **Nbhist = MatGetInt(hist);
   for(int i = 0; i < nbRow; i++){
     for(int j = 0; j < nbCol; j++){
       gris=Gris[i][j];
       Nbhist[0][gris]++;
     }
   }
   return hist;
 }


/*
 * Représentation d'un histogramme sous forme d'une image
 * Entrées : histogramme (matrice de int 1 x 256) et nombre de lignes de
 * l'image résultat (une échelle des niveaux de gris de 25 lignes est ajoutée
 * sous l'histogramme)
 * Sortie : image de niveaux de gris résultat
 */
Image Hist2Im(Matrix Hist, int NbLig)
{
	unsigned char **I;
	int *h,i,j,Max=0,NbCol=256,NbLig2=NbLig+25;
	Image Res;

	if (MatType(Hist)!=Int) return NULL;
  NbLig2=NbLig+25;
	Res=ImCAlloc(GrayLevel,NbLig2,NbCol);
  if (Res==NULL) return NULL;
	h=*MatGetInt(Hist);
	for (j=0;j<NbCol;j++)
		if (h[j]>Max) Max=h[j];
	I=ImGetI(Res);
	for (j=0;j<256;j++)
		for (i=NbLig-1;i>=(NbLig-NbLig*h[j]/Max);i--)
		    I[i][j]=255;
  for (j=0;j<256;j++)
    I[NbLig][j]=0;
  for (i=NbLig+1;i<NbLig2;i++)
    for (j=0;j<256;j++)
      I[i][j]=j;
	return Res;
}


/*
 * Calcul du seuil d'Otsu
 * Entrée : histogramme (matrice de int 1 x 256)
 * Sortie : seuil (niveau de gris) obtenu par la méthode d'Otsu
 */
unsigned char Otsu(Matrix Hist)
{
  Matrix hisCumu =
}


/*
 * Calcul de l'histogramme cumulé à partir de l'histogramme
 * Entrée : histogramme (matrice de int 1 x 256)
 * Sortie : histogramme cumulé (matrice de int 1 x 256)
 */
Matrix Hist2CumHist(Matrix Hist)
{
  AFAIRE(Hist2CumHist);
}


/*
 * Application d'une transformation ponctuelle à une image de niveaux de gris
 * Entreés : image initiale (niveaux de gris) et
 * transformation ponctuelle (matrice de int 1 x 256)
 * Sortie : image de niveaux de gris transformée
 */
Image AppLUT(Image Im, Matrix LUT)
{
  AFAIRE(AppLUT);
}


/*
 * Spécification d'histogramme
 * Entrées : histogramme cumulé de l'image et histogramme cumulé desiré
 * (on suppose que le dernier élément des deux histogrammes cumulés sont
 * les mêmes, c'est-à-dire qu'ils décrivent des images contenant le même nombre
 * de pixels)
 * Sortie : transformation ponctuelle (matrice 1 x 256)
 */
Matrix HistSpecif(Matrix CumHist, Matrix DesCumHist)
{
  AFAIRE(HistSpecif);
}


/*
 * Vérification de la validité d'une matrice représentant un élément
 * structurant binaire (pour l'érosion, la dilatation, etc.)
 * Entrée : matrice représentant un élément structurant
 * Sortie : 0 si la matrice est valide,
            SE_NOT_ODD si son nombre de lignes ou de colonnes n'est pas impair
            SE_NOT_INT si elle ne contient pas que des entiers
            SE_NOT_BIN si elle ne contient pas que des 0 et des 1
*/
int NotValidBinSE(Matrix StructuringElement)
{
  int **ES,NbLig,NbCol,i,j;

  if (MatType(StructuringElement)!=Int)
    return SE_NOT_INT;
  NbLig=MatNbRow(StructuringElement);
	if ((NbLig%2)!=1)
	  return SE_NOT_ODD;
  NbCol=MatNbCol(StructuringElement);
	if ((NbCol%2)!=1)
	  return SE_NOT_ODD;
  ES=MatGetInt(StructuringElement);
  for (i=0;i<NbLig;i++)
    for (j=0;j<NbCol;j++)
      if (ES[i][j]!=0 && ES[i][j]!=1)
        return SE_NOT_BIN;
  return 0;
}


/*
 * Vérification de la validité d'une matrice représentant un élément
 * structurant ternaire (pour la transformation "tout ou rien")
 * Entrée : matrice représentant un élément structurant
 * Sortie : 0 si la matrice est valide,
            SE_NOT_ODD si son nombre de lignes ou de colonnes n'est pas impair
            SE_NOT_INT si elle ne contient pas que des entiers
            SE_NOT_TERN si elle ne contient pas que des 0, des 1 et des 2
*/
int NotValidTernSE(Matrix StructuringElement)
{
  int **ES,NbLig,NbCol,i,j;

  if (MatType(StructuringElement)!=Int)
    return SE_NOT_INT;
  NbLig=MatNbRow(StructuringElement);
	if ((NbLig%2)!=1)
	  return SE_NOT_ODD;
  NbCol=MatNbCol(StructuringElement);
	if ((NbCol%2)!=1)
	  return SE_NOT_ODD;
  ES=MatGetInt(StructuringElement);
  for (i=0;i<NbLig;i++)
    for (j=0;j<NbCol;j++)
      if (ES[i][j]!=0 && ES[i][j]!=1 && ES[i][j]!=2)
        return SE_NOT_TERN;
  return 0;
}


/*
 * Érosion d'une image binaire
 * Entrées : image binaire initiale et élément structurant (matrice de int
 * contenant uniquement des 0 et des 1)
 * Sortie : image binaire erodée
 */
Image Erosion(Image Im, Matrix StructuringElement)
{
  AFAIRE(Erosion);
}
