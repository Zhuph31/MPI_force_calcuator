#include <gflags/gflags.h>
#include <stdexcept>

#include "src/csv/csv_reader.h"
#include "src/serial/serial.h"

DEFINE_int32(mode, 0, "mode number");
DEFINE_int32(particle_num, 100, "number of particles to calculate");
DEFINE_int32(thread_num, 10, "number of threads in mode 1");

int main(int argc, char *argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  printf("particle num:%d\n", FLAGS_particle_num);

  auto particles = read_csv(FLAGS_particle_num);

  std::vector<double> forces;

  switch (FLAGS_mode) {
  case 0:
    calculate_closest(particles);
    forces = calculate_force(particles);
    break;
  case 1:
    break;
  default:
    throw std::runtime_error("this mode is not implemented");
  }

  for (size_t i = 0; i < forces.size(); ++i) {
    printf("index:%lu, particle:%s, force:%e\n", i,
           particles[i].to_string().c_str(), forces[i]);
  }

  return 0;
}
