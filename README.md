# parallel_assignment_1
coee for 1747 parallel programming assignment 1

# BUILD
This project is build with CMake.
```
mkdir -p build
cd build
cmake ..
make
```
Then you can run the program through command ```./force_calculation```

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

