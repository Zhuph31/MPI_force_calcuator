#pragma once

#include <memory>
#include <mpi.h>

#include "../even_threads/even_dispatcher.h"
#include "../particle.h"
#include "tag.h"

class MPIDispatcher {
public:
  MPIDispatcher(int worker_num) : worker_num_(worker_num) {}

  std::vector<double> run(const std::vector<Particle> &particles,
                          std::vector<double> &forces) {
    debug_printf("MPI dispatcher running\n");

    std::vector<std::pair<int, int>> worker_particle_num;
    worker_particle_num.reserve(worker_num_);
    int split_size = particles.size() / worker_num_;
    int left = particles.size() - (split_size * worker_num_);

    int begin = 0, end = -1;
    // whether or not there's a extra on each end, to help calculate the force
    int extra_begin = 0, extra_end = 1;

    using namespace std::chrono;
    auto partition_data_bb = high_resolution_clock::now();
    for (int i = 1; i <= worker_num_; i++) {
      int inner_split_size = split_size;
      if (left > 0) {
        --left;
        ++inner_split_size;
      }
      end = begin + inner_split_size + extra_begin;
      if (end >= particles.size() - 1) {
        end = particles.size() - 1;
        extra_end = 0;
      }

      worker_particle_num.emplace_back(begin, end);

      debug_printf(
          "MPI dispatcher, dispatching for worker %d, begin:%d, end:%d, "
          "inner split size:%d\n",
          i, begin, end, inner_split_size);
      int size = end - begin + 1;
      MPI_Send(&size, 1, MPI_INT, i, MPI_Tag::INT, MPI_COMM_WORLD);
      debug_printf("vec size %d dispatched to worker %d\n", size, i);

      // send extra size
      MPI_Send(&extra_begin, 1, MPI_INT, i, MPI_Tag::INT, MPI_COMM_WORLD);
      MPI_Send(&extra_end, 1, MPI_INT, i, MPI_Tag::INT, MPI_COMM_WORLD);

      for (int j = begin; j <= end; ++j) {
        const Particle &p = particles[j];
        MPI_Send(&(p.x), 1, MPI_INT, i, MPI_Tag::INT, MPI_COMM_WORLD);
        MPI_Send(&(p.y), 1, MPI_INT, i, MPI_Tag::INT, MPI_COMM_WORLD);
        MPI_Send(p.type.c_str(), 2, MPI_CHAR, i, MPI_Tag::CHAR, MPI_COMM_WORLD);
      }
      debug_printf("vec dispatched to worker %d\n", i);

      begin = end - 1;
      extra_begin = 1;
    }
    printf("partition data cost:%ld\n",
           duration_cast<milliseconds>(high_resolution_clock::now() -
                                       partition_data_bb)
               .count());

    long total_cost = 0;

    for (int i = 1; i <= worker_num_; i++) {
      int extra_begin = (i == 1 ? 0 : 1),
          extra_end = (i == worker_num_ ? 0 : 1);
      int size = worker_particle_num[i - 1].second -
                 worker_particle_num[i - 1].first + 1 - extra_begin - extra_end;
      debug_printf("calculate size :%d,%d,%d,%d,%d\n",
                   worker_particle_num[i - 1].second,
                   worker_particle_num[i - 1].first, 1, extra_begin, extra_end);

      int begin = worker_particle_num[i - 1].first + extra_begin;
      debug_printf(
          "MPI dispatcher receive result from worker %d, begin:%d, size:%d\n",
          i, begin, size);

      MPI_Recv(&(forces[begin]), size, MPI_DOUBLE, i, MPI_Tag::DOUBLE,
               MPI_COMM_WORLD, MPI_STATUS_IGNORE);

      long cal_cost;
      MPI_Recv(&cal_cost, 1, MPI_LONG, i, MPI_Tag::LONG, MPI_COMM_WORLD,
               MPI_STATUS_IGNORE);
      total_cost += cal_cost;
    }

    printf("MPI calcuation cost :%ld\n", total_cost / worker_num_);

    // debug_printf("debug results after all:\n%s\n",
    //              string_printf_vector(forces).c_str());

    return forces;
  }

private:
  int worker_num_;
};