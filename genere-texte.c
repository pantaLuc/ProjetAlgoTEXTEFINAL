
/*

PROJET ALGORITHME DU TEXTE QUESTION 1

Ichraq Elaidi 
Safae Jerradi
Luc Panta

*/
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include <time.h>
#include "genere-texte.h"


int main(){
    int taille_texte,tailleAlphabet;
    printf("Entrez la longueur de Texte Voulu:\t");
    scanf("%d", &taille_texte);
    printf("\nSaisissez la taille de l'alphabet:\t");
    scanf("%d", &tailleAlphabet);
    printf("\n");
    genere_texte(taille_texte, tailleAlphabet);
    return 0;
}
void genere_texte( int taille_texte, int tailleAlphabet)
{
    int deplacement;
    char *alphabet;

    clock_t start_t, end_t, total_t;
   int i;

   start_t = clock();
    if( tailleAlphabet == 0) {
        printf(" Alphabet est vide \n");
    } else {

        // construction de l'alphabet
        alphabet  = (char*)malloc(sizeof(char)*tailleAlphabet);

        deplacement = 0;
        while (deplacement < tailleAlphabet)
        {
            alphabet[deplacement] = (char)(97 + deplacement);
            deplacement += 1;
        }
       
        // génération du texte 
        srand((unsigned int) (time(NULL)));
        int indice = 0 ;
       while ( indice <taille_texte )
       {
         printf("%c", alphabet[rand() % tailleAlphabet]);
         indice+=1;  
       }
       
        
       
        free(alphabet);
    }

    end_t = clock();
    printf("temps  fin d' execution , end_t = %ld\n", end_t);
    total_t = (end_t - start_t);
    double time_taken = ((double) total_t)/CLOCKS_PER_SEC;
    printf("Temps d' execution mis Par  CPU: %f \n", time_taken);
}

