#!/usr/bin/env bash
#SBATCH --account cs-302
#SBATCH --qos cs-302
#SBATCH --partition academic
#SBATCH --nodes 1
#SBATCH --ntasks 1
#SBATCH --cpus-per-task 64
#SBATCH --mem 64G
#SBATCH --time 00:01:00

module purge
module load gcc

echo STARTING AT `date`

gcc -O3 -Wall -std=gnu99 -fopenmp rmm.c -o rmm
gcc -O3 -Wall -std=gnu99 -fopenmp pi.c -o pi
gcc -O3 -Wall -std=gnu99 -fopenmp sharing.c -o sharing
gcc -O3 -Wall -std=gnu99 -fopenmp integral.c -o integral
srun ./pi 1 1000000000
srun ./pi 2 1000000000
srun ./pi 4 1000000000
srun ./pi 8 1000000000
srun ./pi 16 1000000000
srun ./pi 32 1000000000
srun ./pi 48 1000000000
srun ./pi 64 1000000000
srun ./integral 1 1000000000 5 9 
srun ./integral 2 1000000000 5 9 
srun ./integral 4 1000000000 5 9 
srun ./integral 8 1000000000 5 9 
srun ./integral 16 1000000000 5 9 
srun ./integral 32 1000000000 5 9 
srun ./integral 48 1000000000 5 9 
srun ./integral 64 1000000000 5 9
srun ./sharing 1 100000000 32
srun ./sharing 2 100000000 32
srun ./sharing 4 100000000 32
srun ./sharing 8 100000000 32 
srun ./rmm 1 1024 1024 1024 0
srun ./rmm 2 1024 1024 1024 0
srun ./rmm 4 1024 1024 1024 0
srun ./rmm 8 1024 1024 1024 0
srun ./rmm 16 1024 1024 1024 0
srun ./rmm 32 1024 1024 1024 0

echo FINISHED at `date`