#pragma once
#include <cassert>
#include <functional>
#include <thread>
#include <vector>

#include "../particle.h"
#include "../serial/serial.h"

class EvenDispatcher {

public:
  EvenDispatcher(int thread_num) : thread_num_(thread_num) {}

  virtual void run(std::vector<Particle> &particles,
                   std::vector<double> &forces, int begin = -1, int end = -1) {
    if (begin < 0 || end < 0) {
      begin = 0;
      end = particles.size() - 1;
    }
    int total_size = end - begin + 1;
    int part_size = total_size / thread_num_;
    if (part_size <= 0) {
      part_size = 1;
    }

    int left = total_size - (part_size * thread_num_);

    printf("running even dispatcher, thread num:%d, size:%d, part_size:%d\n",
           thread_num_, total_size, part_size);

    for (size_t i = 0; i < thread_num_; ++i) {
      int inner_part_size = part_size;
      if (left > 0) {
        --left;
        ++inner_part_size;
      }

      int end = begin + inner_part_size - 1;
      if (end >= particles.size()) {
        end = particles.size() - 1;
      }
      workers_.emplace_back(std::thread(
          std::bind(&EvenDispatcher::calculate, this, i, std::ref(particles),
                    std::ref(forces), begin, end)));
      begin += inner_part_size;
    }

    for (size_t i = 0; i < thread_num_; ++i) {
      workers_[i].join();
    }
  }

protected:
  void calculate(int index, std::vector<Particle> &particles,
                 std::vector<double> &forces, int begin, int end) {
    printf("thread %d starts, begin:%d, end:%d\n", index, begin, end);
    calculate_closest(particles, begin, end);
    debug_particles(particles);
    calculate_force(particles, forces, begin, end);
    printf("%s\n", string_printf_vector(forces).c_str());
    printf("thread %d finishes\n", index);
  }

  int thread_num_ = 1;
  std::vector<std::thread> workers_;
};