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

  void run(std::vector<Particle> &particles, std::vector<double> &forces) {
    int part_size = particles.size() / thread_num_;
    int begin = 0;

    for (size_t i = 0; i < thread_num_; ++i) {
      int end = begin + part_size;
      if (end >= particles.size()) {
        end = particles.size() - 1;
      }
      workers_.emplace_back(std::thread(
          std::bind(&EvenDispatcher::calculate, this, i, std::ref(particles),
                    std::ref(forces), begin, end)));
      begin += part_size + 1;
    }

    for (size_t i = 0; i < thread_num_; ++i) {
      workers_[i].join();
    }
  }

  void calculate(int index, std::vector<Particle> &particles,
                 std::vector<double> &forces, int begin, int end) {
    printf("thread %d starts, begin:%d, end:%d\n", index, begin, end);
    calculate_closest(particles, begin, end);
    calculate_force(particles, forces, begin, end);
    printf("thread %d finishes\n", index);
  }

private:
  int thread_num_ = 1;
  std::vector<std::thread> workers_;
};