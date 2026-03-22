/*
============================================================================
Filename    : rmm.c
Author      : Neha Chakraborty & Guillaume Marie Lepin
SCIPER      : 373384 & 381189
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
   
    /* Step 1: Read the values of M, N and K */
    int num_threads = atoi(argv[1]);
    int M = atoi(argv[2]);
    int N = atoi(argv[3]);
    int K = atoi(argv[4]);
    int debug = atoi(argv[5]);
 
    if(M % 2 != 0 || N % 2 != 0 || K % 2 != 0) {
        printf("M, N and K must be even\n");
        return 1;
    }
 
    /* Step 2: Initialize matrices */
    int *matA[M];
    int *matB[N];
    int *matC[M/2];
 
    init_mat(matA, M, N, 0);
    init_mat(matB, N, K, 1);       
    init_mat(matC, M/2, K/2, -1);
 
    if(debug) {
        display_matrix(matA, M, N, "A");
        display_matrix(matB, N, K, "B");
    }
 
    /* Step 3: Optimized RMM computation with blocking to reduce true sharing */
    printf("Starting Computation...\n");
    set_clock();
    omp_set_num_threads(num_threads);

    // Use tile-based blocking to reduce true sharing:
    
    int BLOCK_SIZE = 16;  // Cache-friendly block size
    
    #pragma omp parallel for collapse(2) 
    for(int i = 0; i < M/2; i += BLOCK_SIZE) {
        for(int j = 0; j < K/2; j += BLOCK_SIZE) {
        
            
            // Each thread processes a BLOCK_SIZE x BLOCK_SIZE region
            int i_end = (i + BLOCK_SIZE < M/2) ? i + BLOCK_SIZE : M/2;
            int j_end = (j + BLOCK_SIZE < K/2) ? j + BLOCK_SIZE : K/2;
            
            for(int i1 = i; i1 < i_end; i1++) {
                for(int j1 = j; j1 < j_end; j1++) {
                    
                    int *A0 = matA[i1*2];
                    int *A1 = matA[i1*2 + 1];

                    int col0 = j1*2;
                    int col1 = j1*2 + 1;

                    int sum = 0;
                    for(int k = 0; k < N; k++) {
                        int a0_val = A0[k];
                        int a1_val = A1[k];
                        int b_col0 = matB[k][col0];
                        int b_col1 = matB[k][col1];
                        
                        sum += a0_val * b_col0;
                        sum += a0_val * b_col1;
                        sum += a1_val * b_col0;
                        sum += a1_val * b_col1;
                    }

                    matC[i1][j1] = sum;
                }
            }
        }
    }
 
    double totaltime = elapsed_time();
 
    /* Step 4: Output */
    printf("Computation Done!\n");
 
    if(debug)
        display_matrix(matC, M/2, K/2, "C");
 
    printf("- Using %d threads: matC computed in %.4gs.\n", num_threads, totaltime);
 
    write_csv(matC, M/2, K/2, "matC.csv");
 
    return 0;
}