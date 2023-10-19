#include <gflags/gflags.h>

#include "src/calculate.h"
#include "src/csv/csv_reader.h"

DEFINE_int32(particle_num, 100, "number of particles to calculate");

int main(int argc, char *argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  printf("particle num:%d\n", FLAGS_particle_num);

  auto particles = read_csv(FLAGS_particle_num);
  calculate_closest(particles);
  auto forces = calculate_force(particles);

  for (int i = 0; i < forces.size(); ++i) {
    printf("index:%d, particle:%s, force:%e\n", i,
           particles[i].to_string().c_str(), forces[i]);
  }

  return 0;
}
