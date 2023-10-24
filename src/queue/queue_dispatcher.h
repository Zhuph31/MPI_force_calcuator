#pragma once
#include <mutex>
#include <queue>
#include <thread>

#include "../even_threads/even_dispatcher.h"
#include "../particle.h"

class QueueDispatcher : public EvenDispatcher {
 public:
  QueueDispatcher(int id, int thread_num, int chunk_size)
      : EvenDispatcher(thread_num), id_(id), chunk_size_(chunk_size) {}

  void run(std::vector<Particle> &particles, std::vector<double> &forces,
           int begin = -1, int end = -1) override {
    int particle_size = particles.size();
    int pos = 0;
    int extra_begin = (begin == 0 ? 0 : 1), extra_end = 1;

    debug_printf("queue disptacher %d start running\n", id_);
    bool should_break = false;
    while (!should_break) {
      std::vector<Particle> chunk;
      chunk.reserve(chunk_size_ + 2);
      int count = 0, begin_index = pos;
      int expect_size = chunk_size_ + extra_begin + extra_end;
      while (count < expect_size && pos <= end) {
        chunk.emplace_back(particles[pos]);
        ++count;
        ++pos;
        if (count < expect_size && pos > end) {
          extra_end = 0;
        }
      }

      if (count < expect_size && pos < particle_size) {
        // extra end, the final one that does not need to be computed
        chunk.emplace_back(particles[pos]);
        extra_end = 1;
        should_break = true;
      }

      printf("pushed chunk %d|%zu|%d|%d, pos:%d\n", begin_index, chunk.size(),
             extra_begin, extra_end, pos);
      q_.push(
          ParticleChunk(begin_index, extra_begin, extra_end, std::move(chunk)));

      if (pos >= particle_size) {
        break;
      }

      // extra begin
      pos -= extra_end + 1;
      extra_begin = 1;
    }

    debug_printf("queue dispatcher %d finsihed chunking\n", id_);

    for (int i = 0; i < thread_num_; ++i) {
      workers_.emplace_back(std::thread(std::bind(
          &QueueDispatcher::fetch_and_calculate, this, i, std::ref(forces))));
    }

    for (size_t i = 0; i < thread_num_; ++i) {
      workers_[i].join();
    }

    debug_printf("queue dispatcher %d finished\n", id_);
  }

 private:
  struct ParticleChunk {
    int begin_index;
    int extra_begin;
    int extra_end;
    std::vector<Particle> chunk;
    ParticleChunk() {}
    ParticleChunk(int bi, int eb, int ee, std::vector<Particle> &&c) {
      begin_index = bi;
      extra_begin = eb;
      extra_end = ee;
      chunk = std::move(c);
    }
  };

  std::pair<bool, ParticleChunk> fetch() {
    std::lock_guard<std::mutex> l(m_);
    if (q_.empty()) {
      return {false, ParticleChunk()};
    }

    auto ret = q_.front();
    q_.pop();
    return {true, ret};
  }

  void fetch_and_calculate(int index, std::vector<double> &forces) {
    auto ret = fetch();
    if (!(ret.first)) {
      return;
    }
    ParticleChunk &pc = ret.second;

    std::vector<double> partial_forces;
    partial_forces.resize(pc.chunk.size());
    calculate(index, pc.chunk, partial_forces, pc.extra_begin,
              pc.chunk.size() - 1 - pc.extra_end);

    // copy back to forces
    for (int i = pc.extra_begin, j = pc.begin_index + pc.extra_begin;
         i < partial_forces.size() - pc.extra_end; ++i, ++j) {
      forces[j] = partial_forces[i];
    }
  }
  int id_;
  int chunk_size_;
  std::mutex m_;
  std::queue<ParticleChunk> q_;
};