#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>


#define BATCH_SIZE 20
#define MEM_SIZE 10000
#define ERROR 0.1
#define QUANTILE 1.96



void compute_std(int* trans, int* rho, int num_of_samples, double mean_trans, double mean_rho, double* error_trans, double* error_rho);
void compute_trajecotry(eca_t* eca, int grid_size, int *transient, int* rho, uint8_t* grid, double * time_of_insert);
void compute_global_properties(eca_t* eca, int grid_size);