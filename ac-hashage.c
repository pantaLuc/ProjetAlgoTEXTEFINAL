

#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "hash.h"


#define MAN() fprintf(stderr, "SYNTAX: %s <words file> <text file>\n", argv[0])


int main(int argc, char **argv) {
    clock_t start_t, end_t, total_t;
   start_t = clock();

    if (argc != 3) {
        fprintf(stderr, "Not permitted, wrong number of arguments\n");
        MAN();
        exit(EPERM);
    }

    char *wfile = argv[1];
    char *tfile = argv[2];
    
    //read words
    char *words_buff,*text;
    char **words = NULL;
    char delim[] = "\n";
    int n_spaces = 0,i;
    size_t maxNodes = 0;
    
    words_buff = lireFichierParLigne(wfile);
    text = lireFichierParLigne(tfile);
    
    if (words_buff)
    {
        words_buff = strtok(words_buff, delim);
        while (words_buff != NULL)
        {
            words = realloc (words, sizeof (char*) * ++n_spaces);
            if (words == NULL){
                fprintf(stderr, "Memory allocation error\n");
                exit(ENOMEM);
            }
            
            words[n_spaces-1] = words_buff;
            maxNodes += strlen(words[n_spaces-1]);
            words_buff = strtok (NULL, delim);
        }
    }else{
        fprintf(stderr, "File error, words file not found or empty\n");
        exit(EIO);
    }
    maxNodes = maxNodes + 1;
    free(words_buff);

    //create trie
    Trie trie = creerLeTrie(maxNodes);
    
    //Add words to trie
    for (i = 0; i < n_spaces; ++i){
        AjoutMotTrie(trie, words[i]);
    }
    
    //construct substitute table
    calculerSupleant(trie);
    
    //search words
    //acSearch(trie,text);
    printf ("%d\n", i);
    end_t = clock();
    printf("temps  fin d' execution , end_t = %ld\n", end_t);
    total_t = (end_t - start_t);
    double time_taken = ((double) total_t)/CLOCKS_PER_SEC;
    printf("Temps d' execution mis Par  CPU: %f \n", time_taken);
    
    
}