#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "ECA_glob_prop.h"
#include <math.h>


void compute_std(int* trans, int* rho, int num_of_samples, double mean_trans, double mean_rho, double* error_trans, double* error_rho){
    double std_trans=0;
    double std_rho=0;
    for (int i=0; i<num_of_samples; i++){
        std_trans=std_trans+pow(trans[i]-mean_trans, 2);
        std_rho=std_rho+pow(rho[i]-mean_rho, 2);
    }
    std_trans=std_trans/num_of_samples;
    std_rho=std_rho/num_of_samples;
    *error_trans=(QUANTILE*std_trans)/(sqrt(num_of_samples)*mean_trans);
    *error_rho=(QUANTILE*std_rho)/(sqrt(num_of_samples)*mean_rho);
}

void compute_trajecotry(eca_t* eca, int grid_size, int *transient, int* rho, uint8_t* grid, double * time_of_insert){
    trie_t *chunk=create_multiple_tries();
    int index=0;

    trie_t *trie=&chunk[index];
    index++;

    int t=0;
    insert_to_trie_and_check(trie, grid, grid_size, transient, t, &chunk, &index);

    bool visited=false;

    while (visited==false){
        t++;
        grid=simulate(eca, 1, grid, grid_size);
        visited=insert_to_trie_and_check(trie, grid, grid_size, transient, t, &chunk, &index);
    }
    *rho=t;

    printf("\n\ntransient=%i, rho=%i \n", *transient, *rho);
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
        compute_std(trans_arr, rho_arr, num_of_samples, mean_trans, mean_rho, &error_trans, &error_rho);
        printf("error trans: %f, error rho: %f", error_trans, error_rho);
    }

    printf("\navg trans: %f, avg rho: %f\n", mean_trans, mean_rho);
    
}
