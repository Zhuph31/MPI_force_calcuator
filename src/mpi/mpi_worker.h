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

    MPI_Recv(particles.data(), vector_size * sizeof(Particle), MPI_BYTE, 0,
             MPI_Tag::VECTOR, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("worker %d vec received\n", mpi_rank_);

    for (const auto &each : particles) {
      printf("debug:%s\n", each.to_string().c_str());
    }

    // std::vector<double> forces;
    // EvenDispatcher dispatcher(thread_num_);
    // dispatcher.run(particles, forces);
  }

private:
  int mpi_rank_;
  int thread_num_;
};