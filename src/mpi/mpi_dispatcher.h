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
    printf("MPI dispatcher running\n");
    for (int i = 1; i <= worker_num_; i++) {
      printf("MPI dispatcher, dispatching for worker %d\n", i);
      int particle_num = particles.size();
      MPI_Send(&particle_num, 1, MPI_INT, i, MPI_Tag::INT, MPI_COMM_WORLD);
      printf("vec size dispatched to worker %d\n", i);

      // std::vector<int> particle_position;
      // std::vector<std::
      // particle_position.reserve(particles.size() * 2);
      for (const auto &p : particles) {
        MPI_Send(&(p.x), 1, MPI_INT, i, MPI_Tag::INT, MPI_COMM_WORLD);
        MPI_Send(&(p.y), 1, MPI_INT, i, MPI_Tag::INT, MPI_COMM_WORLD);
        MPI_Send(p.type.c_str(), 2, MPI_CHAR, i, MPI_Tag::INT, MPI_COMM_WORLD);
      }
      printf("vec dispatched to worker %d\n", i);
    }

    std::vector<double> forces;

    return forces;
  }

private:
  int worker_num_;
};