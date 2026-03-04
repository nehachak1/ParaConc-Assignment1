/*
============================================================================
Filename    : rmm.c
Author      : Neha Chakraborty & Guillaume Marie Lepin
SCIPER		: 373384 & 381189
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "utility.h"

int main(int argc, char *argv[]) {
    if(argc != 6) {
        printf("Usage: %s <nthreads> <M> <N> <K> <0|1>\n", argv[0]);
        return 1;
    }
    
    /* Step 1: Read the values of M, N and K from the command line arguments. */
    int num_threads = atoi(argv[1]);
    int M = atoi(argv[2]);
    int N = atoi(argv[3]);
    int K = atoi(argv[4]);
    int debug = atoi(argv[5]);

    if(M % 2 != 0 || N % 2 != 0 || K % 2 != 0) {
        printf("M, N and K must be even\n");
        return 1;
    }

    /* Step 2: Generates and initializes matrices A and B with random values. */
    int *matA[M];
    int *matB[N];
    int *matC[M/2];

    init_mat(matA, M, N, 0);
    init_mat(matB, N, K, 1);        
    init_mat(matC, M/2, K/2, -1);   // -1 indicates that matrix is initialized with 0s

    if(debug) {
        display_matrix(matA, M, N, "A");
        display_matrix(matB, N, K, "B");
    }

    /* Step 3: Computes the matrix C as the RMM of matrices A and B. */

    /* Parallelize and optimize this part only! */
    printf("Starting Computation...\n");
    set_clock();

    omp_set_num_threads(num_threads);

                             // parallelizes both loops as one large iteration
    #pragma omp parallel for collapse(2)
    for(int idx = 0; idx < M/2; idx++) {
        for(int jdx = 0; jdx < K/2; jdx++) {

            // row pointers for A because otherwise loop would repeatedly comupte the address
            int *A0 = matA[idx*2];
            int *A1 = matA[idx*2 + 1];

            // two columns in B 
            int col0 = jdx*2;
            int col1 = col0 + 1;

            int sum = 0;

            // 4 dot-products worth of multiplies/adds (
            for(int kdx = 0; kdx < N; kdx++) {
                int b0 = matB[kdx][col0];
                int b1 = matB[kdx][col1];

                sum += A0[kdx] * b0;
                sum += A0[kdx] * b1;
                sum += A1[kdx] * b0;
                sum += A1[kdx] * b1;
            }

            matC[idx][jdx] = sum;

        }
    }
    double totaltime = elapsed_time();

    /* Step 4: Write matrix C into a csv file matC.csv and exit. */
    printf("Computation Done!\n");
    if(debug)
        display_matrix(matC, M/2, K/2, "C");
    printf("- Using %d threads: matC computed in %.4gs.\n", num_threads, totaltime);
    write_csv(matC, M/2, K/2, "matC.csv");
}