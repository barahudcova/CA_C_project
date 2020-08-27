#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>

#define BATCH_SIZE 20
#define MEM_SIZE 10000
#define ERROR 0.1
#define QUANTILE 1.96

typedef struct eca_t {
    uint8_t rule;
    uint8_t* rule_table;
} eca_t;

eca_t* create_eca(uint8_t rule);
void init_eca(eca_t* eca);
uint8_t* generate_random_initial_grid(int grid_size);
uint8_t* generate_initial_grid_from_num(int number, int grid_size);
void print_grid(uint8_t* grid, int grid_size);
uint8_t* simulate(eca_t* eca, int tim, uint8_t* grid, int grid_size);
void compute_error(int* trans, int* rho, int num_of_samples, double mean_trans, double mean_rho, double* error_trans, double* error_rho);
void compute_trajecotry(eca_t* eca, int grid_size, int *transient, int* rho, uint8_t* grid, double * time_of_insert);
void compute_global_properties(eca_t* eca, int grid_size);
