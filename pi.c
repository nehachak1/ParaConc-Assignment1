/*
============================================================================
Filename    : pi.c
Author      : Neha Chakraborty & Guillaume Marie Lepin
SCIPER		: 373384    
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <omp.h>
#include "utility.h"

double calculate_pi (int num_threads, int samples);

int main (int argc, const char *argv[]) {

    int num_threads, num_samples;
    double pi;

    if (argc != 3) {
		printf("Invalid input! Usage: ./pi <num_threads> <num_samples> \n");
		return 1;
	} else {
        num_threads = atoi(argv[1]);
        num_samples = atoi(argv[2]);
	}

    set_clock();
    pi = calculate_pi (num_threads, num_samples);

    printf("- Using %d threads: pi = %.15g computed in %.4gs.\n", num_threads, pi, elapsed_time());

    return 0;
}


double calculate_pi (int num_threads, int samples) {

    if (samples <= 0 || num_threads <= 0) return 0.0;

    // how many random points fall inside circle
    long long inside = 0; // long used since samples can be very large

    #pragma omp parallel num_threads(num_threads) // create the threads
    { // runs in parallel
        int tid = omp_get_thread_num();

        // different random sequences per thread
        rand_gen gen = init_rand((unsigned int)(tid + 1)); 

        // splits the loop iterations among threads
                        // each thread keeps a private copy of inside - prevent race conditions
        #pragma omp for reduction(+:inside) 
        // each thread runs its assigned chunk of iterations
        for (long long i = 0; i < (long long)samples; i++) {
            // random point inside unit square 
            double x = next_rand(gen);   
            double y = next_rand(gen);  
            // distance from Origin
            double r2 = x * x + y * y;
            // check if Inside Circle
            if (r2 <= 1.0) inside++;
        }

        // rees memory for the current thread’s random generator
        free_rand(gen);
    }

    return 4.0 * (double)inside / (double)samples;

}
