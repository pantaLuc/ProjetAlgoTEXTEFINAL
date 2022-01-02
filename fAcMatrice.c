#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "fAcMatrice.h"
#include "file.h"

#define LSIZ 128 
#define RSIZ 10 

/* */

char* lireFichierParLigne(char *fichier)
{
    char *buffer = NULL;
   int string_size, read_size;
   FILE *handler = fopen(fichier, "r");

   if (handler)
   {
       fseek(handler, 0, SEEK_END);
       string_size = ftell(handler);
       rewind(handler);

       // Allocate string
       buffer = (char*) malloc(sizeof(char) * (string_size + 1) );

       read_size = fread(buffer, sizeof(char), string_size, handler);

       buffer[string_size] = '\0';

       if (string_size != read_size)
       {
           free(buffer);
           buffer = NULL;
       }
       fclose(handler);
    }

    return buffer;
}

/* ---------------------------------------------------------------------
   createTrie()
   ---------------------------------------------------------------------
   Role : create new trie
   ---------------------------------------------------------------------
   returns Trie
   --------------------------------------------------------------------- */
Trie creerLeTrie(int maxNode){
    if (maxNode < 1) {
        return NULL;
    }
    struct _trie *nouveauTrie = malloc(sizeof *nouveauTrie);
    if (nouveauTrie == NULL) {
        return NULL;
    }
    nouveauTrie -> maxNode = maxNode;
    nouveauTrie -> nextNode = 1;    // implicitement, la racine du trie est 0
    nouveauTrie -> transition = NULL;
    nouveauTrie -> finaux = NULL;
    nouveauTrie -> supleant = NULL;
    
    nouveauTrie -> transition = malloc(
        (size_t)maxNode  * sizeof (*nouveauTrie -> transition));
    if (nouveauTrie -> transition == NULL) {
        goto error;
    }
    for (int **p = nouveauTrie -> transition; 
            p < nouveauTrie -> transition + maxNode;
            ++p) {
        *p = malloc(((int)UCHAR_MAX + 1) * sizeof **p);
        if (*p == NULL) {
            goto error;
        }
        // initialisation de la matrice à -1 (aucune transition)
        for (int *q = *p; q <= *p + UCHAR_MAX; ++q) {
            *q = -1;
        }
    }
    
    nouveauTrie -> finaux = malloc((size_t)maxNode * sizeof (Finaux));
    if (nouveauTrie -> finaux == NULL) {
        goto error;
    }
    for (Finaux *p = nouveauTrie -> finaux; p < nouveauTrie -> finaux + maxNode; ++p) {
        p->nombreSortie = 0;
        p->sorties = NULL;
    }

    nouveauTrie -> supleant = malloc((size_t)maxNode * sizeof *nouveauTrie -> supleant);
    if (nouveauTrie -> supleant == NULL) {
        goto error;
    }
    for (int *t = nouveauTrie -> supleant;
            t < nouveauTrie -> supleant + maxNode; ++t) {
        *t = 0;
    }
        
    goto success;
    error:
    libererTrie(&nouveauTrie);
    success:
    return nouveauTrie;
}

/* ---------------------------------------------------------------------
   trieAdd()
   ---------------------------------------------------------------------
   Role : add word to trie
   ---------------------------------------------------------------------
   trie: Trie
   w: String
   --------------------------------------------------------------------- */
void AjoutMotTrie(Trie trie, unsigned char *w) {
    int etatAncien = 0;
    int  etatCourant= 0;
    unsigned char *p = w;
    size_t nouvelleTaille = 0;
    
    while (*p != '\0' && etatCourant != -1) {
        etatAncien = etatCourant;
        etatCourant = prochainEtat(trie, etatCourant, *p);
        
        ++p;
    }
    if (etatCourant == -1) {
        etatCourant = etatAncien;
        --p;
        if (strlen((char*)p) > (size_t)(trie -> maxNode - trie -> nextNode)) {
            printf("deboudement,noeud ne peut etre rajoute\n");
            return;
        }
        
        while (*p != '\0') {
            ajouterTransition(trie, etatCourant, *p, trie -> nextNode);
            etatCourant = trie -> nextNode;
            trie -> nextNode++;
            ++p;
        }
         
    }
    if(trie -> finaux[etatCourant].nombreSortie == 0){
        trie -> finaux[etatCourant].sorties = malloc(sizeof (char *));
        trie -> finaux[etatCourant].sorties[0] = malloc(sizeof (w));
        //strcpy(trie->exits[curr_state].outs[0],(char *) w);
        trie->finaux[etatCourant].nombreSortie ++;
    }
}

/* ---------------------------------------------------------------------
   freeTrie()
   ---------------------------------------------------------------------
   Role : free allocated memory of the trie
   ---------------------------------------------------------------------
   trie: Trie
   --------------------------------------------------------------------- */
void libererTrie(struct _trie** trie) {
    if (*trie != NULL) {
        for (int **p = (*trie) -> transition;
                p < (*trie) -> transition + (*trie) -> maxNode;
                ++p) {
            free(*p);
        }
        free((*trie) -> transition);
        free((*trie) -> finaux);
        free((*trie) -> supleant);
        free(*trie);
        *trie = NULL;
    }
}

/* ---------------------------------------------------------------------
   nextState()
   ---------------------------------------------------------------------
   Role : get next state from node with the letter
   ---------------------------------------------------------------------
   trie: struct _trie* 
   node: int
   letter: char
   returns the next state if it exists, -1 else
   --------------------------------------------------------------------- */
int prochainEtat(struct _trie *trie, int noeud, unsigned char lettre) {
    return trie->transition[noeud][lettre];
}

/* ---------------------------------------------------------------------
   addTransition()
   ---------------------------------------------------------------------
   Role : add new transition to trie
   ---------------------------------------------------------------------
   trie: struct _trie* 
   startNode: int
   targetNode: int
   letter: char
   returns 0 when success, -1 else
   --------------------------------------------------------------------- */
int ajouterTransition(struct _trie *trie, int noeudDepart,
        unsigned char lettre, int noeudCible) {
    
    trie->transition[noeudDepart][lettre] = noeudCible;
    return 0;
}


/* ---------------------------------------------------------------------
   transitions()
   ---------------------------------------------------------------------
   Role : get all the transition from the node
   ---------------------------------------------------------------------
   trie: struct _trie*
   node: int
   listsize: int, will be set to the list of transition length
   mode: mode (0: we don't accept that startnode = targetnode, 1: all)
   returns list of transitions
   --------------------------------------------------------------------- */
int *transitions(struct _trie *trie, int node, int *listsize, int mode){
    int *l = NULL;
    int i = 0;
    size_t j = 0;
    
    for (int *q = trie->transition[node]; q <= trie->transition[node] + UCHAR_MAX; ++q) {
        if(*q != -1 && ((mode == 0 && *q != node) || mode != 0)){
            l = realloc(l, i * sizeof (int));
            l[i] = j;
            i++;
        }
        j++;
    }
    *listsize = i;
    return l;
}

/* ---------------------------------------------------------------------
   mergeExits()
   ---------------------------------------------------------------------
   Role : merge two exits
   ---------------------------------------------------------------------
   src: Exit
   dest: Exit
   dest = dest U src
   --------------------------------------------------------------------- */
void mergeExits(Finaux *src, Finaux *dest){
    size_t src_size = 0, dest_size = 0;
    size_t j = 0;
    if(src->nombreSortie != 0){
        src_size = src->nombreSortie;
        dest_size = dest->nombreSortie;

        dest->sorties = realloc(dest->sorties, (src_size + dest_size) * sizeof (char *));
        
        for(size_t i = dest_size;i < dest_size + src_size;i++){
            dest->sorties[i] = malloc(sizeof (src->sorties[j]));
            strcpy(dest->sorties[i],src->sorties[j]);
            j++;
        }
        dest->nombreSortie = dest_size + src_size;
    }
}


/* ---------------------------------------------------------------------
   calculateSup()
   ---------------------------------------------------------------------
   Role : calculate Aho-corasik subtitudes table
   ---------------------------------------------------------------------
   trie: Trie
   --------------------------------------------------------------------- */
void calculerSupleant(Trie trie){
    File *file = init();
    size_t i = 0,j = 0;
    char lettreCourant;
    int r,s,listeSize = 0;
    int *l = NULL,curr_ind,curr_target;
    
    
    l = transitions(trie, 0, &listeSize,0);
    for(i = 0; i<listeSize;i++) {
        curr_ind = l[i];
        curr_target = trie->transition[0][curr_ind];
        lettreCourant = (char) curr_ind;
        enfiler(file,curr_target);
        trie->supleant[curr_target] = 0;
    }
    free(l);
    while (!estVideFile(*file)){
        r = defiler(file);
        l = transitions(trie, r, &listeSize,1);
        for(i = 0; i<listeSize;i++) {
            curr_ind = l[i];
            curr_target = trie->transition[r][curr_ind];
            lettreCourant = (char) curr_ind;
            enfiler(file,curr_target);
            s = trie->supleant[r];
            while(prochainEtat(trie,s,lettreCourant) == -1 && s != 0){
                s = trie->supleant[s];
            }
            s = prochainEtat(trie,s,lettreCourant);
            if(s != -1){
                trie->supleant[curr_target] = s;
                if(trie->finaux[s].nombreSortie != 0){
                    mergeExits(&(trie->finaux[s]),&(trie->finaux[curr_target]));
                }
            }

        }
    }
    
    
}

/* ---------------------------------------------------------------------
   acSearch()
   ---------------------------------------------------------------------
   Role : perform Aho-corasik search in text using trie
   ---------------------------------------------------------------------
   trie: Trie
   text: char *
   returns numbers of hits
   --------------------------------------------------------------------- */
int rechercheAhoCorasik(Trie trie, char *text){
    int e = 0;
    int cpt = 0;
    size_t exitSize = 0;
    for(int j = 0; j < strlen(text);j++){
        while(prochainEtat(trie,e,text[j]) == -1 && e != 0){
            e = trie->supleant[e];
        }
        e = prochainEtat(trie,e,text[j]);
        if(e != -1 && trie->finaux[e].nombreSortie != 0){
            cpt += trie->finaux[e].nombreSortie;
        }else if(e == -1){
            e = 0;
        }
    }
    return cpt;
}






















