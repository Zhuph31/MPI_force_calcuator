#pragma once
#include <mpi.h>

#include "../even_threads/even_dispatcher.h"
#include "../particle.h"
#include "tag.h"

class MPIWorker {
public:
  MPIWorker(int thread_num) : thread_num_(thread_num) {}

  void run() {
    int vector_size = 0;
    MPI_Recv(&vector_size, 1, MPI_INT, 0, MPI_Tag::INT, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);

    std::vector<Particle> particles;
    particles.resize(vector_size);
    MPI_Recv(particles.data(), vector_size * sizeof(Particle), MPI_BYTE, 0,
             MPI_Tag::VECTOR, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    MPI_Recv(&vector_size, 1, MPI_INT, 0, MPI_Tag::INT, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);

    for (const auto &each : particles) {
      printf("debug:%s\n", each.to_string().c_str());
    }

    // std::vector<double> forces;
    // EvenDispatcher dispatcher(thread_num_);
    // dispatcher.run(particles, forces);
  }

private:
  int thread_num_;
};