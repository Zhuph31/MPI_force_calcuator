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
    printf("worker %d vec size received\n", mpi_rank_);

    std::vector<Particle> particles;
    particles.resize(vector_size);

    for (int i = 0; i < vector_size; ++i) {
      MPI_Recv(&(particles[i].x), 1, MPI_INT, 0, MPI_Tag::INT, MPI_COMM_WORLD,
               MPI_STATUS_IGNORE);
      MPI_Recv(&(particles[i].y), 1, MPI_INT, 0, MPI_Tag::INT, MPI_COMM_WORLD,
               MPI_STATUS_IGNORE);

      char message_buffer[2];
      MPI_Recv(&message_buffer, 2, MPI_CHAR, 0, MPI_Tag::CHAR, MPI_COMM_WORLD,
               MPI_STATUS_IGNORE);
      particles[i].type = message_buffer;
      printf("worker %d vec received, size:%lu\n", mpi_rank_, particles.size());
    }

    printf("try to debug\n");
    for (const auto &each : particles) {
      printf("debug:%d,%d,%s\n", each.x, each.y, each.type.c_str());
    }

    // std::vector<double> forces;
    // EvenDispatcher dispatcher(thread_num_);
    // dispatcher.run(particles, forces);
  }

private:
  int mpi_rank_;
  int thread_num_;
};