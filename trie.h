#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>
#include <time.h>

#define BATCH 32

typedef struct trie_t {
    struct trie_t *child[2];
    int first_visit;
} trie_t;

trie_t* create_trie(void);
trie_t* create_multiple_tries(void);
bool insert_to_trie_and_check(trie_t *trie, uint8_t *el, int grid_size, int *first_visit, int t, trie_t** chunk, int* index, int* cm_ct, trie_t *** chunk_mem, int * size_of_cm);
void free_trie(trie_t* trie);