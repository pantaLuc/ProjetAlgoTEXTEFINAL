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
    File *file = malloc(sizeof(*file));
    file->premier = NULL;

    return file;
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

/* Le défilage ressemble étrangement au dépilage. 
    Étant donné qu'on possède un pointeur vers le premier élément de la file, 
            il nous suffit de l'enlever et de renvoyer sa valeur.

 */
int defiler(File *file)
{
    if (file == NULL)
    {
        exit(EXIT_FAILURE);
    }

    int nombreFile=0;

    if (file->premier != NULL)
    {
        ElementFile *elementDefile = file->premier;

        nombreFile= elementDefile->nombre;
        file->premier = elementDefile->suivant;
        free(elementDefile);
    }

    return nombreFile;
}

/*La maniere la plus simple de savoir si notre file est vide de connaitre 
 Le contenu du premier element 

 */

int estVideFile(File file){        
    if(file.premier == NULL) return 1;
    else return 0;
}