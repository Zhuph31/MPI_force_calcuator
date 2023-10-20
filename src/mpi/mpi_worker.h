#pragma once
#include <mpi.h>

#include "../even_threads/even_dispatcher.h"
#include "../particle.h"

class MPIWorker {
public:
  MPIWorker(int thread_num) : thread_num_(thread_num) {}

  void run(std::vector<Particle> &particles, std::vector<double> &forces,
           int begin, int end) {
    EvenDispatcher dispatcher(thread_num_);

    // create sub view of the vector
    auto begin_iter = particles.begin() + begin,
         end_iter = particles.begin() + end + 1;
    // auto particle_view =

    dispatcher.run(particles, forces);
  }

private:
  int thread_num_;
};