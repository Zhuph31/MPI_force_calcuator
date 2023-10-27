#!/bin/bash

echo mode 1
./force_calculator --mode=1 --particle_num=100000000
echo mode 2
./force_calculator --mode=2 --particle_num=100000000 --thread_num=16
echo mode 3
mpirun -np 5 ./force_calculator --mode=3 --particle_num=100000000 --thread_num=4 --chunk_size=10000