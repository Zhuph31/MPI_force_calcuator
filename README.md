# MPI force calculator
A parallel program used to calculate force between different nodes using MPI.

# BUILD
This project is build with CMake.
```
mkdir -p build
cd build
cmake ..
make
```
Then you can run the program through command ```./force_calculator```

# RUN
This program has 3 modes.
Mode 1 is serial calculation.
Mode 2 is multiple threads calculation.
Mode 3 is multiple process & multiple threads calculation.
## Parameters
- mode: control the mode, possible selections are 1, 2 and 3.
- particle_num: number of particles to be calculated.
- show result: whether or not print the calculation result in the end of the program.
- thread_num: number of threads, only effective in mode 2 and mode 3.
- chunk_size: the size of data chunk, only effective in mode 3.

# Usage
Running the program in mode 1 and mode 2 only requires you to sepcifies the above parameters.
Running the program in mode 3 requires you to execute the program through command mpirun:
```mpirun -np ${number_of_processes} ./force_calculation```
After running, the time cost of each part of the program will be printed through standard output.

# Test Cases
- runnig ```./force_calculator --mode=1 --particle_num=100 --show_result``` will calculate and print the first 100 particles' result using mode 1.
- runnig ```./force_calculator --mode=2 --particle_num=100000000 --thread_num=${thread_num}``` can allow you to test the performance of mode 2 with different number of threads.
- runnig ```mpirun -np ${process_num} ./force_calculator --mode=3 --particle_num=${particle_num} --thread_num=${thread_num}``` can allow you to test the performance of mode 3 with different number of processes, threads, and particles. Note that the main process will not participate in calculation so the number of leader processes is ${process_num}-1.
- running ```bash compare_modes.sh``` will run the program in all 3 modes with the same number of particles, and the same number of threads for mode 2 and 3, and output the time cost.
- running ```bash test_mode3.sh``` will run the program in mode 3 with different number of particles.

