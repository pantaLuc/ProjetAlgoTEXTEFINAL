
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
//#include "genere-texte.h"
#include "genere-mots.h"

int main() {

    /*int taille_texte,tailleAlphabet;
    printf("Entrez la longueur de Texte Voulu:\t");
    scanf("%d", &taille_texte);
    printf("\nSaisissez la taille de l'alphabet:\t");
    scanf("%d", &tailleAlphabet);
    printf("\n");
    genere_texte(taille_texte, tailleAlphabet);*/
    int nbr_mot, lon_min, lon_max, taille_alphabet; 

    printf("Saisissez la nombre de mot à générer:\t");
    scanf("%d", &nbr_mot);
    printf("\nSaisissez la longueur minimale:\t");
    scanf("%d", &lon_min);
    printf("\nSaisissez la longueur maximale:\t");
    scanf("%d", &lon_max);
    printf("\nSaisissez la taille de l'alphabet:\t");
    scanf("%d", &taille_alphabet);

   genere_mots(nbr_mot, lon_min, lon_max, taille_alphabet);

    return 0;
}