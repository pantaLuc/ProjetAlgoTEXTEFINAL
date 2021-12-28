#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include "file.h"

/* ---------------------------------------------------------------------
   init()
   ---------------------------------------------------------------------
   Role : create empty fifo
   ---------------------------------------------------------------------
   Returns pointer to fifo
   --------------------------------------------------------------------- */
File *init()
{
    ElementFile *nouveau = malloc(sizeof(*nouveau));
    nouveau->suivant = NULL;

    return nouveau;
}

/*  La fonction d'enfilage ajoute un élément à la file.

Il y a deux cas à gérer :

   La file est vide : on crée la file en faisant pointer premier vers le nouvel élément créé.

    La file n'est pas vide : on parcourt toute la file en partant du premier élément jusqu'à arriver au dernier. On rajoutera notre nouvel élément après le dernier.
*/
void enfiler(File *file, int nouveauNBR)
{
    ElementFile *nouvelElement = malloc(sizeof(*nouvelElement));
    if (file == NULL || nouvelElement == NULL)
    {
        exit(EXIT_FAILURE);
    }

    nouvelElement->nombre = nouveauNBR;
    nouvelElement->suivant = NULL;

    if (file->premier != NULL) 
    {
        ElementFile *elementActuel = file->premier;
        while (elementActuel->suivant != NULL)
        {
            elementActuel = elementActuel->suivant;
        }
        elementActuel->suivant = nouvelElement;
    }
    else 
    {
        file->premier = nouvelElement;
    }
}

/* ---------------------------------------------------------------------
   fifoGet()
   ---------------------------------------------------------------------
   Role : get first element from fifo
   ---------------------------------------------------------------------
   fifo: Fifo
   returns first element
   --------------------------------------------------------------------- */
int defiler(File *file)
{
    if (file == NULL)
    {
        exit(EXIT_FAILURE);
    }

    int nombreFile=0;

    if (file->premier != NULL)
    {
        ElementFile *outElement = fifo->first;

        s = outElement->element;
        fifo->first = outElement->next;
        free(outElement);
    }

    return s;
}

/* ---------------------------------------------------------------------
   fifoEmpty()
   ---------------------------------------------------------------------
   Role : check if fifo is empty
   ---------------------------------------------------------------------
   fifo: Fifo
   returns 1 if fifo is empty, 0 else
   --------------------------------------------------------------------- */
int fifoEmpty(Fifo fifo){
    if(fifo.first == NULL) return 1;
    else return 0;
}