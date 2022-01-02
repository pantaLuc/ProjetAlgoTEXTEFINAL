#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "hash.h"
#include "file.h"



char* lireFichierParLigne(char *filename)
{
   char *buffer = NULL;
   int string_size, read_size;
   FILE *handler = fopen(filename, "r");

   if (handler)
   {
       fseek(handler, 0, SEEK_END);
       string_size = ftell(handler);
       rewind(handler);

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
Trie creerLeTrie(int maxNode) {
    if (maxNode < 1) {
        return NULL;
    }
    struct _trie *newTrie = malloc(sizeof *newTrie);
    if (newTrie == NULL) {
        return NULL;
    }
    newTrie -> maxNode = maxNode;
    newTrie -> nextNode = 1;    
    newTrie -> transition = NULL;
    newTrie -> finaux = NULL;
    
    size_t tableSize = TABLE_SIZE(maxNode);
    newTrie -> transition = malloc(tableSize * sizeof *newTrie -> transition);
    if (newTrie -> transition == NULL) {
        goto error;
    }
    for (List *t = newTrie -> transition;
            t < newTrie -> transition + tableSize; ++t) {
        *t = NULL;
    }
    
    newTrie -> finaux = malloc((size_t)maxNode * sizeof (Finaux));
    if (newTrie -> finaux == NULL) {
        goto error;
    }
    for (Finaux *p = newTrie -> finaux; p < newTrie -> finaux + maxNode; ++p) {
        p->nombreSortie = 0;
        p->sorties = NULL;
    }
    
    newTrie -> supleant = malloc(tableSize * sizeof *newTrie -> supleant);
    if (newTrie -> supleant == NULL) {
        goto error;
    }
    for (int *t = newTrie -> supleant;
            t < newTrie -> supleant + tableSize; ++t) {
        *t = 0;
    }
    
    goto success;
    error:
    libererTrie(&newTrie);
    success:
    return newTrie;
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
    int old_state = 0;
    int curr_state = 0;
    unsigned char *p = w;
    size_t newSize = 0;
    
    while (*p != '\0' && curr_state != -1) {
        old_state = curr_state;
        curr_state = prochainEtat(trie, curr_state, *p);
        
        ++p;
    }

    if (curr_state == -1) {
        curr_state = old_state;
        --p;
        if (strlen((char*)p) > (size_t)(trie -> maxNode - trie -> nextNode)) {
            printf("overflow, node could not be inserted \n");
            return;
        }
        while (*p != '\0') {
            AjoutDeTransition(trie, curr_state, *p, trie -> nextNode);
            curr_state = trie -> nextNode;
            trie -> nextNode++;
            ++p;
        }
    }
    
    if(trie -> finaux[curr_state].nombreSortie == 0){
        trie -> finaux[curr_state].sorties = malloc(sizeof (char *));
        trie -> finaux[curr_state].sorties[0] = malloc(sizeof (w));
        //strcpy(trie->exits[curr_state].outs[0],w);
        trie->finaux[curr_state].nombreSortie ++;
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
        size_t tableSize = TABLE_SIZE((*trie) -> maxNode);
        for (List *t = (*trie) -> transition;
                t < (*trie) -> transition + tableSize; ++t) {
            while (*t != NULL) {
                struct _list *tmp = *t;
                *t = (*t) -> next;
                free(tmp);
            }
        }
        free((*trie) -> transition);
        free((*trie) -> finaux);
        free(*trie);
        *trie = NULL;
    }
}

/* ---------------------------------------------------------------------
   hash()
   ---------------------------------------------------------------------
   Role : get the index in hash table
   ---------------------------------------------------------------------
   node: int
   letter: char
   --------------------------------------------------------------------- */
size_t hash(int node, unsigned char letter) {
    return (size_t)node * ((size_t)UCHAR_MAX + 1) + (size_t)letter;
}

/* ---------------------------------------------------------------------
   prochainEtat()
   ---------------------------------------------------------------------
   Role : get next state from node with the letter
   ---------------------------------------------------------------------
   trie: struct _trie* 
   node: int
   letter: char
   returns the next state if it exists, -1 else
   --------------------------------------------------------------------- */
int prochainEtat(struct _trie *trie, int node, unsigned char letter) {
    size_t index = hash(node, letter) % TABLE_SIZE(trie -> maxNode);
    int next = -1;
    struct _list *t = trie -> transition[index];
    while (t != NULL && next == -1) {
        if (t -> startNode == node && t -> letter == letter) {
            next = t -> targetNode;
        }
        t = t -> next;
    }
    
    return next;
}

/* ---------------------------------------------------------------------
   AjoutDeTransition()
   ---------------------------------------------------------------------
   Role : add new transition to trie
   ---------------------------------------------------------------------
   trie: struct _trie* 
   startNode: int
   targetNode: int
   letter: char
   returns 0 when success, -1 else
   --------------------------------------------------------------------- */
int AjoutDeTransition(struct _trie *trie, int startNode,
        unsigned char letter, int targetNode) {
    struct _list *newTransition = malloc(sizeof *newTransition);
    if (newTransition == NULL) {
        return -1;
    }
    newTransition -> startNode = startNode;
    newTransition -> targetNode = targetNode;
    newTransition -> letter = letter;
    
    size_t tableSize = TABLE_SIZE(trie -> maxNode);
    size_t index = hash(startNode, letter) % tableSize;
    newTransition -> next = trie -> transition[index];
    trie -> transition[index] = newTransition;
    return 0;
}

/* ---------------------------------------------------------------------
   tableState()
   ---------------------------------------------------------------------
   Role : display transition table state informations
   ---------------------------------------------------------------------
   trie: struct _trie* 
   --------------------------------------------------------------------- */
void tableState(struct _trie *trie) {
    size_t tableSize = TABLE_SIZE(trie -> maxNode);
    
    size_t longestList = 0;
    size_t count = 0;
    for (struct _list **p = trie -> transition;
            p < trie -> transition + tableSize; ++p) {
        size_t len = 0;
        for (struct _list *q = *p; q != NULL; q = q -> next) {
            count++;
            len++;
        }
        if (len > longestList) {
            longestList = len;
        }
    }

    double loadFactor = ((double)count) / (double) tableSize;

    printf("Size : %lu\n"
           "Load factor : %lf\n"
           "Load factor max : %lf\n"
           "Depth : %lu\n",tableSize,loadFactor,LOAD_FACTOR,longestList);
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
List *transitions(struct _trie *trie, int node, int *listsize, int mode){
    List *l = NULL;
    int i = 0;
    size_t tableSize = TABLE_SIZE(trie->maxNode);
    
    int next=0;
    struct _list *current = NULL;
    
    for (List *t = trie -> transition;t < trie -> transition + tableSize; ++t) {
        if(*t != NULL){
            current = *t;
            while (current != NULL) {
                if((current->startNode == node) && ((mode == 0 && current->startNode != current->targetNode) ||mode != 0)){
                    l = realloc(l, i * sizeof *t);
                    l[i] = current;
                    i++;
                }
                current = current -> next;
            }

        }
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
   calculatesupleant()
   ---------------------------------------------------------------------
   Role : calculate Aho-corasik subtitudes table
   ---------------------------------------------------------------------
   trie: Trie
   --------------------------------------------------------------------- */
void calculerSupleant(Trie trie){
    File *file = init();
    size_t tableSize = TABLE_SIZE(trie->maxNode);
    size_t i = 0;
    int r,s,listeSize = 0;
    List *l = NULL;
    List current = NULL;
    
    l = transitions(trie, 0, &listeSize,0);

    for(i = 0; i<listeSize;i++) {
        current = l[i];  
        enfiler(file,current->targetNode);
        trie->supleant[current->targetNode] = current->startNode;
            
    }
    while (!estVideFile(*file)){
        r = defiler(file);
        l = transitions(trie, r, &listeSize,1);
        for(i = 0; i<listeSize;i++) {
            current = l[i];
            enfiler(file,current->targetNode);
            s = trie->supleant[current->startNode];

            while(prochainEtat(trie,s,current->letter) == -1 && s != 0){
                s = trie->supleant[s];                
            }
            s = prochainEtat(trie,s,current->letter);
            if(s != -1){
                trie->supleant[current->targetNode] = s;
                if(trie->finaux[s].nombreSortie != 0){
                    mergeExits(&(trie->finaux[s]),&(trie->finaux[current->targetNode]));
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
int acSearch(Trie trie, char *text){
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
