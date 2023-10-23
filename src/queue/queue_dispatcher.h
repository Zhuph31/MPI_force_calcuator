#pragma once
#include <mutex>
#include <queue>
#include <thread>

#include "../even_threads/even_dispatcher.h"
#include "../particle.h"

class QueueDispatcher : public EvenDispatcher {
public:
  QueueDispatcher(int thread_num, int chunk_size)
      : EvenDispatcher(thread_num), chunk_size_(chunk_size) {}

  // unfortunately begin and end are not used currently in this function
  void run(std::vector<Particle> &particles, std::vector<double> &forces,
           int begin = -1, int end = -1) override {
    int particle_size = particles.size();
    int pos = 0;
    int extra_begin = 0, extra_end = 1;
    while (1) {
      std::vector<Particle> chunk;
      chunk.reserve(chunk_size_);
      int count = 0, begin_index = pos;
      while (count < chunk_size_ && pos < particle_size) {
        chunk.emplace_back(particles[pos]);
        ++count;
        ++pos;
      }
      if (pos < particle_size) {
        // extra end
        chunk.emplace_back(particles[pos]);
      } else {
        extra_end = 0;
      }

      q_.push(
          ParticleChunk(begin_index, extra_begin, extra_end, std::move(chunk)));

      if (pos >= particle_size) {
        break;
      }

      // extra begin
      --pos;
      extra_begin = 1;
    }
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
  int chunk_size_;
  std::mutex m_;
  std::queue<ParticleChunk> q_;
};