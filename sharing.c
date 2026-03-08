/*
============================================================================
Filename    : pi.c
Author      : Neha Chakraborty & Guillaume Marie Lepin
SCIPER		: 373384
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include "utility.h"

int perform_buckets_computation(int, int, int);

int main (int argc, const char *argv[]) {
    int num_threads, num_samples, num_buckets;

    if (argc != 4) {
		printf("Invalid input! Usage: ./sharing <num_threads> <num_samples> <num_buckets> \n");
		return 1;
	} else {
        num_threads = atoi(argv[1]);
        num_samples = atoi(argv[2]);
        num_buckets = atoi(argv[3]);
	}
    
    set_clock();
    perform_buckets_computation(num_threads, num_samples, num_buckets);

    printf("Using %d threads: %d operations completed in %.4gs.\n", num_threads, num_samples, elapsed_time());
    return 0;
}

/* Parallelize and optimise this function */
int perform_buckets_computation(int num_threads, int num_samples, int num_buckets) {    
    int *histogram = (int*) calloc(num_buckets, sizeof(int));

    #pragma omp parallel num_threads(num_threads)
    {
        int thread_id = omp_get_thread_num();
        rand_gen generator = init_rand(thread_id);

        int private_histogram[num_buckets];

        #pragma omp for
        for(int i = 0; i < num_samples; i++){
            int val = next_rand(generator) * num_buckets;
            histogram[val]++;
        }
        free_rand(generator);

        #pragma omp critical
        for(int b = 0; b < num_buckets; b++){
            histogram[b] += private_histogram[b];
        }
    }
    free(histogram);
    
    return 0;
}

