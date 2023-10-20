#pragma once
#include <vector>

#include "utils/utils.h"

struct Particle {
  Particle() { closest = nullptr; }
  Particle(int _x, int _y, const std::string &_type)
      : x(_x), y(_y), type(_type), closest(nullptr) {}
  int x;
  int y;
  std::string type;

  // the particle that is closest to the currnet particle
  Particle *closest;

  std::string to_string() const {
    std::string closest_str =
        closest ? string_printf("%d|%d|%s", closest->x, closest->y,
                                closest->type.c_str())
                : "";
    return string_printf("%d|%d|%s-%s", x, y, type.c_str(),
                         closest_str.c_str());
  }
};

void debug_particles(const std::vector<Particle> &particles);
