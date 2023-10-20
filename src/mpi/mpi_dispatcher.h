#pragma once

#include <memory>
#include <mpi.h>

#include "../even_threads/even_dispatcher.h"
#include "../particle.h"
#include "tag.h"

class MPIDispatcher {
public:
  MPIDispatcher(int worker_num) : worker_num_(worker_num) {}

  std::vector<double> run(const std::vector<Particle> &particles) {
    for (int i = 1; i <= worker_num_; i++) {
      int particle_num = particles.size();
      MPI_Send(&particle_num, 1, MPI_INT, worker_num_, MPI_Tag::INT,
               MPI_COMM_WORLD);
      MPI_Send(particles.data(), particle_num * sizeof(Particle), MPI_BYTE, 1,
               MPI_Tag::VECTOR, MPI_COMM_WORLD);
    }

    std::vector<double> forces;

    return forces;
  }

private:
  int worker_num_;
};