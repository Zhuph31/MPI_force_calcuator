#!/bin/bash

for i in 10000 100000 1000000 10000000 100000000 
do
    mpirun -np 5 ./force_calculator --mode=3 --particle_num=$i --thread_num=8 --chunk_size=10000
done