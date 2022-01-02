#ifndef FACMATRICE_H
#define FACMATRICE_H

/*
 *-----------------------------------------
 * Exit struct
 *-----------------------------------------
 */
typedef struct Finaux Finaux;
struct Finaux{
    int nombreSortie;       
    char **sorties;           

};

/* a l aide du TP1*/
typedef struct _trie *Trie;
struct _trie {
    int maxNode;        
    int nextNode;       
    int **transition;  
    Finaux *finaux;       
    int *supleant;           
};


/*lesfonctions*/

char* lireFichierParLigne(char *ficher);                                       
Trie creerLeTrie(int maxNode);                                     
void AjoutMotTrie(Trie trie, unsigned char *mot);                                
void libererTrie(struct _trie** trie);                                   
int prochainEtat(struct _trie *trie, int noeud, unsigned char lettre);   
int ajouterTransition(struct _trie *trie, int noeudDepart, unsigned char lettre, int noeudCible);              
void calculerSupleant(Trie trie);                       
int rechercheAhoCorasik(Trie trie, char *text);                                 


#endif