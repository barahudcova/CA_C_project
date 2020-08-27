#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "trie.h"
#include <time.h>

trie_t* create_trie(void){
    trie_t *trie=malloc(sizeof(trie_t));
    trie->child[0]=NULL;
    trie->child[1]=NULL;
    trie->first_visit=0;
    return trie;
}

trie_t* create_multiple_tries(void){
    trie_t* c=calloc(BATCH, sizeof(trie_t));

    return c;
}

bool insert_to_trie_and_check(trie_t *trie, uint8_t *el, int grid_size, int *first_visit, int t, trie_t** chunk, int* index, int* cm_ct, trie_t *** chunk_mem, int* size_of_cm){
    //printf("inserting to trie\n");
    bool saved=true;
    if (trie==NULL){
        saved=false;
    }
    int j =0;
    while (j<grid_size){
        if (trie->child[*el]==NULL){
            trie->child[*el]=&(*chunk)[*index];
            *index=*index+1;
            //printf("index: %i, chunk: %p\n", *index, &(*chunk)[*index]);
            //printf("chunk mem count: %i\n", *cm_ct);
            if (*index==BATCH){
                *chunk=create_multiple_tries();
                (*chunk_mem)[*cm_ct]=*chunk;
                printf("new chunk at address: %p\n", &(*chunk_mem)[*cm_ct]);
                *cm_ct=*cm_ct+1;
                printf("size of chunk mem: %lu\n", sizeof(*chunk_mem)/sizeof(*chunk));
                printf("chunk counter: %i\n", *cm_ct);
                // printf("chunk mem counter increased\n");
                // printf("cm ct: %i\n", *cm_ct);
                if (*cm_ct==*size_of_cm){
                    printf("chunk overflowing");
                    *size_of_cm=*size_of_cm+128;
                    *chunk_mem=realloc(*chunk_mem, *size_of_cm*sizeof(trie_t *));
                }
                *index=0;
            }
            saved=false;
        }
        // printf("shifting trie\n");
        trie=trie->child[*el];
        // printf("trie shifted\n");
        j++;
        el++;
    }

    if (saved==false){
        //printf("saving first visit\n");
        trie->first_visit=t;
        //printf("saving ok\n");
    }
    else {
        *first_visit=trie->first_visit;
    }
    //printf("trie first visit: :%i\n", trie->first_visit);
    
    return saved;
}

void free_trie(trie_t* trie){
    if (trie->child[0]==NULL && trie->child[1]==NULL){
        printf("im here nothing\n");
        free(trie);
        trie=NULL;
        return;
    }
    else if (trie->child[0]!=NULL){
        if (trie->child[0]->child[0]==NULL && trie->child[0]->child[1]==NULL){
        printf("im here 0\n");
        free(trie->child[0]);
        trie->child[0]=NULL;
        free_trie(trie);
        }
    }
    else if (trie->child[1]!=NULL){
        if (trie->child[1]->child[0]==NULL && trie->child[1]->child[1]==NULL){
        printf("im here 1\n");
        free(trie->child[1]);
        trie->child[1]=NULL;
        free_trie(trie);
        }
    }
    else {
        printf("recursing\n");
        free_trie(trie->child[0]);
        free_trie(trie->child[1]);
    }
    printf("\n");

}


