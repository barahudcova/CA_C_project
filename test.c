#include <stdio.h>
#include <inttypes.h>
#include "ECA.h"
#include <stdbool.h>
#include "trie.h"
#include "m_twist.h"

clock_t start, end;
double cpu_time_used;

uint8_t* incr_grid(uint8_t* grid, int grid_size, uint8_t arity){
    bool incr=false;
    int i = 0;
    while (incr==false){
        if (grid[i]<arity-1){
            grid[i]+=1;
            incr=true;
        }
        else {
            grid[i]=0;
            i++;
            if (i==grid_size){
                return grid;
            }
        }
    }
    return grid;
}


int main(int argc, char *argv[]){
    if (argc == 3){
        init_genrand64(clock());

        uint8_t rule=atoi(argv[1]);
        int grid_size=atoi(argv[2]);

        // trie_t* trie=create_trie();
        // //trie->child[0]=create_trie();
        // printf("%p\n", trie->child[0]);
        // free_trie(trie);
        // printf("%p\n", trie->child[0]);
        // printf("%p\n", trie);

        trie_t* trie=calloc(1, sizeof(trie_t));
        printf("%p\n", trie->child[0]);

        exit(0);

        int * arr=malloc(2*4);
        printf("%lu", sizeof(arr)/4);
        arr[0]=1;
        printf("%lu", sizeof(arr)/4);
        arr[1]=1;
        printf("%lu", sizeof(arr)/4);

    }

    
    else {
        printf("Wrong number of parameters, ECA rule, grid size.\n");
    }
}