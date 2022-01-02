
#ifndef HASH_H
#define HASH_H
#include <stddef.h>
#define LOAD_FACTOR 0.75
#define TABLE_SIZE(maxNode) (size_t)(((maxNode) - 1) / LOAD_FACTOR + 1)


/*
 *-----------------------------------------
 * List of transitions struct
 *-----------------------------------------
 */
struct _list {
    int startNode;          /* start node of the transition */
    int targetNode;         /* target node of the transition */
    unsigned char letter;   /* label letter of the transition */
    struct _list *next;     /* next link */
};

typedef struct _list *List;


typedef struct Finaux Finaux;
struct Finaux{
    int nombreSortie;       
    char **sorties;           

};

/*
 *-----------------------------------------
 * Trie struct
 *-----------------------------------------
 */
struct _trie {
    int maxNode;        /* max nodes of the trie */
    int nextNode;       /* index of the next node */
    List *transition;   /* table of transition lists */
    Finaux *finaux;        /* exits table */
    int  *supleant;          /* Aho-corasik substitudes table */
};
typedef struct _trie *Trie;


/*
 *-----------------------------------------
 * Functions
 *-----------------------------------------
 */

char* lireFichierParLigne(char *filename);                                         // read file and get it's content
Trie creerLeTrie(int maxNode);                                           // create new trie
void AjoutMotTrie(Trie trie, unsigned char *w);                              // add word to trie
void libererTrie(struct _trie** trie);                                     // free allocated memory for trie
size_t hash(int node_p, unsigned char letter);                          // hash function
int prochainEtat(struct _trie *trie, int node, unsigned char letter);      // get next state from node with letter
int AjoutDeTransition(struct _trie *trie, int startNode,                    
        unsigned char letter, int targetNode);                          // add transition to trie transitions table
void tableDeTransition(struct _trie *trie);                                    // print transition table state informations

void calculerSupleant(Trie trie);                                           // calculate aho-corasik substitutes table
int rechercheAhoCorasik(Trie trie, char *text);                                    // aho-corasik search

#endif