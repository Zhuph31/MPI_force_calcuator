#include "serial.h"

void calculate_closest(std::vector<Particle> &particles, int begin, int end) {
  if (begin < 0 || end < 0) {
    begin = 0;
    end = particles.size() - 1;
  }

  for (int i = begin; i <= end; ++i) {
    Particle &cur = particles[i];
    int closest = -1;
    if (i - 1 < 0) {
      closest = i + 1;
    } else if (i + 1 >= particles.size()) {
      closest = i - 1;
    } else {
      int before_dis = -1, after_dis = -1;
      {
        int x_dis = std::abs(particles[i - 1].x - cur.x);
        int y_dis = std::abs(particles[i - 1].y - cur.y);
        before_dis = std::pow(x_dis, 2) + std::pow(y_dis, 2);
      }
      {
        int x_dis = std::abs(particles[i + 1].x - cur.x);
        int y_dis = std::abs(particles[i + 1].y - cur.y);
        after_dis = std::pow(x_dis, 2) + std::pow(y_dis, 2);
      }
      closest = before_dis < after_dis ? i - 1 : i + 1;
    }

    cur.closest = &particles[closest];
  }
}

void calculate_force(const std::vector<Particle> &particles,
                     std::vector<double> &results, int begin, int end) {
  if (begin < 0 || end < 0) {
    begin = 0;
    end = particles.size() - 1;
  }

  int count = 0;
  for (size_t i = begin; i <= size_t(end); ++i) {
    ++count;
    const Particle &p = particles[i];
    double dist = std::sqrt(std::pow((p.x - p.closest->x) * unit_size, 2) +
                            std::pow((p.y - p.closest->y) * unit_size, 2));

    double force = coulombK * std::pow(charge, 2) / std::pow(dist, 2);
    // results[i] = force;
  }
  // printf("claculate force %d\n", count);
}
