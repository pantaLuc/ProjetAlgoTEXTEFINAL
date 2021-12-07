#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include <time.h>
#include "genere-texte.h"

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

