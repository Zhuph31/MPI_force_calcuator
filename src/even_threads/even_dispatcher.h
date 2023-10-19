#pragma once
#include <thread>
#include <vector>

#include "../particle.h"
#include "../serial/serial.h"

class EvenDispatcher {

public:
  EvenDispatcher(const std::vector<Particle> &particles, int thread_num)
      : thread_num_(thread_num) {
    for (size_t i = 0; i < thread_num; ++i) {
      workers_.emplace_back(std::thread([]() {}));
    }
  }

  int get_thread_num() const { return thread_num_; }

private:
  void calculate(const std::vector<Particle> &particles, int begin, int end) {}

  int thread_num_ = 1;
  std::vector<std::thread> workers_;
};