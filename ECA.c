#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "ECA.h"
#include "m_twist.h"
#include <math.h>
#include "trie.h"

eca_t* create_eca(uint8_t rule){
    eca_t* eca=calloc(1, sizeof(eca_t));
    eca->rule=rule;
    return eca;
}



void init_eca(eca_t* eca){
    uint8_t rule=eca->rule;    
    eca->rule_table=malloc(8*sizeof(uint8_t));
    for (int i=0; i<8; i++){
        eca->rule_table[i]=rule%2;
        rule=rule/2;
    }
}

uint8_t* generate_random_initial_grid(int grid_size){
    uint8_t* grid=calloc(grid_size, sizeof(uint8_t));
        for (int i=0; i<grid_size; i++){
            grid[i]=genrand64_int64()%2;
        } 
    return grid;
}


void print_grid(uint8_t *grid, int grid_size){
    for (int i=0; i<grid_size; i++){
        printf("%d", grid[i]);
    }
    printf("\n");
}

uint8_t* simulate(eca_t* eca, int tim, uint8_t* grid, int grid_size){
    uint8_t *rule_table=eca->rule_table;

    uint8_t *new_grid=calloc(grid_size, sizeof(uint8_t));
    int s1;
    int s2;
    int s3;
    for (int t=0; t<tim; t++){
        for (int i=1; i<grid_size-1; i++){
            s1=grid[(i-1)%grid_size];
            s2=grid[(i)%grid_size];
            s3=grid[(i+1)%grid_size];
            int pos = s1*4+s2*2+s3;
            new_grid[i]=rule_table[pos];
        }
        new_grid[0]=rule_table[4*(grid[grid_size-1])+2*grid[0]+grid[1]];
        new_grid[grid_size-1]=rule_table[4*(grid[grid_size-2])+2*(grid[grid_size-1])+grid[0]];
        for (int i=0; i<grid_size; i++){
            grid[i]=new_grid[i];
        }
       // print_grid(new_grid, grid_size);
    }
    //print_grid(new_grid, grid_size);
    return new_grid;
}

void compute_error(int* trans, int* rho, int num_of_samples, double mean_trans, double mean_rho, double* error_trans, double* error_rho){
    double std_trans=0;
    double std_rho=0;
    for (int i=0; i<num_of_samples; i++){
        std_trans=std_trans+pow(trans[i]-mean_trans, 2);
        std_rho=std_rho+pow(rho[i]-mean_rho, 2);
    }
    std_trans=std_trans/num_of_samples;
    std_trans=sqrt(std_trans);

    std_rho=std_rho/num_of_samples;
    std_rho=sqrt(std_rho);

    //printf("std trans: %f\n", std_trans);
    
    if (mean_trans==0){
        *error_trans=0;
    }
    else {
        *error_trans=(QUANTILE*std_trans)/(sqrt(num_of_samples)*mean_trans);
    }
    if (mean_rho==0){
        *error_rho=0;
    }
    else {
        *error_rho=(QUANTILE*std_rho)/(sqrt(num_of_samples)*mean_rho);
    }
}

void compute_trajecotry(eca_t* eca, int grid_size, int *transient, int* rho, uint8_t* grid, double * time_of_insert){
    trie_t *chunk=create_multiple_tries();
    trie_t **chunk_mem=calloc(128, sizeof(chunk));
    printf("size of chunk: %lu\n", sizeof(chunk));
    printf("size of chunk mem: %lu\n", sizeof(chunk_mem)/sizeof(chunk));
    int size_of_cm=128;
    int cm_ct=0;
    chunk_mem[cm_ct]=chunk;
    cm_ct++;
    // printf("%p\n", chunk_mem[0]);
    // printf("%p\n", chunk);
    // printf("\n");

    int index=0;


    trie_t *trie=&chunk[index];
    index++;

    int t=0;
    printf("inserting to trie\n");
    insert_to_trie_and_check(trie, grid, grid_size, transient, t, &chunk, &index, &cm_ct, &chunk_mem, &size_of_cm);
    printf("insertion ok\n");
    bool visited=false;

    while (visited==false){
        t++;
        printf("t: %i\n", t);
        grid=simulate(eca, 1, grid, grid_size);
        //printf("inserting to trie\n");
        visited=insert_to_trie_and_check(trie, grid, grid_size, transient, t, &chunk, &index, &cm_ct, &chunk_mem, &size_of_cm);
        //printf("insertion ok\n");
    }
    *rho=t;

    // printf("attempting to free memory\n");
    // printf("size of mem chunk: %lu", sizeof(chunk_mem)/sizeof(chunk));
    // printf("chunk memory at final position: %p\n", chunk_mem[cm_ct-1]);
    for (int i=0; i<cm_ct; i++){
        free(chunk_mem[i]);
    }
    free(chunk_mem);
    printf("memory freed, new trans starting to compute\n\n\n");

    
    
    //printf("%p", chunk[0].child[0]);


    //printf("transient=%i, rho=%i \n", *transient, *rho);
}


void compute_global_properties(eca_t* eca, int grid_size){
    double error_trans=1;
    double error_rho=1;

    int num_of_samples=0;

    int sum_trans=0;
    int sum_rho=0; 
    
    double mean_trans, mean_rho;

    int mem_size=MEM_SIZE;

    int *trans_arr = (int*) malloc(4*MEM_SIZE);
    int *rho_arr = (int*) malloc(4*MEM_SIZE);

    double t=0;

    while (error_trans>ERROR || error_rho>ERROR){
        for (int i=0; i<BATCH_SIZE; i++){
            int trans=0;
            int rho=0;
            uint8_t* grid=generate_random_initial_grid(grid_size);
            printf("computing trajecotry: \n\n");
            compute_trajecotry(eca, grid_size, &trans, &rho, grid, &t);
            trans_arr[num_of_samples]=trans;
            rho_arr[num_of_samples]=rho;
            num_of_samples++;
            if (num_of_samples >= mem_size){
                mem_size+=MEM_SIZE;
                trans_arr=realloc(trans_arr, mem_size);
                rho_arr=realloc(rho_arr, mem_size);                
            }
            sum_trans=sum_trans+trans;
            sum_rho=sum_rho+rho;
        }
        mean_trans=sum_trans/num_of_samples;
        mean_rho=sum_rho/num_of_samples;
        compute_error(trans_arr, rho_arr, num_of_samples, mean_trans, mean_rho, &error_trans, &error_rho);
        //printf("error trans: %f, error rho: %f, num of samples: %i", error_trans, error_rho, num_of_samples);
    }

    printf("\navg trans: %f, avg rho: %f, num of samples: %i\n", mean_trans, mean_rho, num_of_samples);
    free(trans_arr);
    free(rho_arr);
}
