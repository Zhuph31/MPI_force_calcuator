#include "particle.h"

void debug_particles(const std::vector<Particle> &particles) {
  for (const auto &p : particles) {
    debug_printf("%s\n", p.to_string().c_str());
  }
}
