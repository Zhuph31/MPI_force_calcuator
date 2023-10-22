#pragma once
#include <mpi.h>

#include "../even_threads/even_dispatcher.h"
#include "../particle.h"
#include "tag.h"

class MPIWorker {
public:
  MPIWorker(int mpi_rank, int thread_num)
      : mpi_rank_(mpi_rank), thread_num_(thread_num) {}

  void run() {
    printf("MPI worker %d started\n", mpi_rank_);
    int vector_size = 0;
    MPI_Recv(&vector_size, 1, MPI_INT, 0, MPI_Tag::INT, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    printf("worker %d vec size received, %d\n", mpi_rank_, vector_size);

    std::vector<Particle> particles;
    particles.resize(vector_size);

    int extra_begin = 0, extra_end = 0;
    MPI_Recv(&extra_begin, 1, MPI_INT, 0, MPI_Tag::INT, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    MPI_Recv(&extra_end, 1, MPI_INT, 0, MPI_Tag::INT, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);

    for (int i = 0; i < vector_size; ++i) {
      MPI_Recv(&(particles[i].x), 1, MPI_INT, 0, MPI_Tag::INT, MPI_COMM_WORLD,
               MPI_STATUS_IGNORE);
      MPI_Recv(&(particles[i].y), 1, MPI_INT, 0, MPI_Tag::INT, MPI_COMM_WORLD,
               MPI_STATUS_IGNORE);

      char message_buffer[2];
      MPI_Recv(&message_buffer, 2, MPI_CHAR, 0, MPI_Tag::CHAR, MPI_COMM_WORLD,
               MPI_STATUS_IGNORE);

      particles[i].type = message_buffer;
      // printf("MPI worker received particle:%s\n",
      //        particles[i].to_string().c_str());
    }
    printf("worker %d vec received, size:%lu, extra_begin:%d, extra_end:%d\n",
           mpi_rank_, particles.size(), extra_begin, extra_end);

    std::vector<double> forces;
    forces.resize(vector_size);
    EvenDispatcher dispatcher(thread_num_);
    int calculate_end = vector_size - 1 - extra_end;
    dispatcher.run(particles, forces, extra_begin, calculate_end);

    printf("calculated, size:%d, begin:%d, end:%d\n", vector_size, extra_begin,
           calculate_end);

    std::string debug_str;
    for (int i = 0; i < particles.size(); ++i) {
      debug_str += string_printf("%d:%s:%lf\n", i,
                                 particles[i].to_string().c_str(), forces[i]);
    }
    printf("worker %d debug result:\n%s\n", mpi_rank_, debug_str.c_str());
  }

private:
  int mpi_rank_;
  int thread_num_;
};