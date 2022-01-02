#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include <time.h>
#include "genere-mots.h"

int main() {

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
void genere_texte( int taille_texte, int tailleAlphabet)
{
    int deplacement;
    char *alphabet;

     // check if our alphabet is free 
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
}
void genere_mots(int nbr_mot, int lon_min, int lon_max, int taille_alphabet)
{
   clock_t start_t, end_t, total_t;
   int i;

   start_t = clock();
  
    
        // génération des mots 
        srand((unsigned int) (time(NULL)));
        printf("Starting of the program, start_t = %ld\n", start_t);

        for(int i = 1; i <= nbr_mot; i ++) {

            //la longueur du mot 
            int long_mot = (rand() % (lon_max +1 - lon_min)) + lon_min;
             genere_texte(long_mot,taille_alphabet);
             printf("\n");

        }

    
    end_t = clock();
    printf("temps execution du programme , end_t = %ld\n", end_t);
    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
     printf("Temps d' execution mis Par  CPU: %ld\n", total_t  );
   
}

