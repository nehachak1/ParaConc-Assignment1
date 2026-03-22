/*
============================================================================
Filename    : sharing.c
Author      : Neha Chakraborty & Guillaume Marie Lepin
SCIPER		: 373384 & 381189
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


int perform_buckets_computation(int num_threads, int num_samples, int num_buckets) {    
    int *histogram = (int*) calloc(num_buckets, sizeof(int));
    if (histogram == NULL) return 1;


    #pragma omp parallel num_threads(num_threads)
    {
        int thread_id = omp_get_thread_num();
        rand_gen generator = init_rand(thread_id);

        int *local_histogram = (int*) calloc(num_buckets, sizeof(int));
        if (local_histogram == NULL) exit(1);

        #pragma omp for
        for (int i = 0; i < num_samples; i++) {
            int val = next_rand(generator) * num_buckets;
            local_histogram[val]++;
        }

        free_rand(generator);

        
    
        for (int b = 0; b < num_buckets; b++) {
            #pragma omp atomic
            histogram[b] += local_histogram[b];
        }
        

        free(local_histogram);
    }

    free(histogram);
    return 0;
}



