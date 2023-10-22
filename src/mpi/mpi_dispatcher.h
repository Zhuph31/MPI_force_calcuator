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

    std::vector<int> worker_particle_num;
    worker_particle_num.reserve(worker_num_);
    int split_size = particles.size() / worker_num_;

    int begin = 0, end = -1;
    // whether or not there's a extra on each end, to help calculate the force
    int extra_begin = 0, extra_end = 1;

    for (int i = 1; i <= worker_num_; i++) {
      end = begin + split_size;
      if (end >= particles.size() - 1) {
        end = particles.size() - 1;
        extra_end = 0;
      }

      printf("MPI dispatcher, dispatching for worker %d, begin:%d, end:%d\n", i,
             begin, end);
      int size = end - begin + 1;
      MPI_Send(&size, 1, MPI_INT, i, MPI_Tag::INT, MPI_COMM_WORLD);
      printf("vec size dispatched to worker %d\n", i);

      // send extra size
      MPI_Send(&extra_begin, 1, MPI_INT, i, MPI_Tag::INT, MPI_COMM_WORLD);
      MPI_Send(&extra_end, 1, MPI_INT, i, MPI_Tag::INT, MPI_COMM_WORLD);

      for (const auto &p : particles) {
        MPI_Send(&(p.x), 1, MPI_INT, i, MPI_Tag::INT, MPI_COMM_WORLD);
        MPI_Send(&(p.y), 1, MPI_INT, i, MPI_Tag::INT, MPI_COMM_WORLD);
        MPI_Send(p.type.c_str(), 2, MPI_CHAR, i, MPI_Tag::CHAR, MPI_COMM_WORLD);
      }
      printf("vec dispatched to worker %d\n", i);

      begin = end - 1;
      extra_begin = 1;
    }

    // for (int i = 1; i <= worker_num_; i++) {
    //   std::vector<double> forces;
    //   forces.resize(particles.size());
    //   MPI_Recv(forces.data(), worker_particle_num[i], MPI_INT, 0,
    //   MPI_Tag::INT,
    //            MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    // }

    std::vector<double> forces;

    return forces;
  }

private:
  int worker_num_;
};