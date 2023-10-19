#include "src/calculate.h"
#include "src/csv/csv_reader.h"

int main() {

  int num = 100;

  auto particles = read_csv(num);
  calculate_closest(particles);
  auto forces = calculate_force(particles);

  for (int i = 0; i < forces.size(); ++i) {
    printf("index:%d, particle:%s, force:%e\n", i,
           particles[i].to_string().c_str(), forces[i]);
  }
  // std::vector<Particle> ps;
  // ps.push_back(Particle(3, 4, "p"));
  // ps.push_back(Particle(3, 2, "p"));
  // ps[0].closest = &ps[1];
  // ps[1].closest = &ps[0];
  // auto forces = calculate_force(ps);

  // for (const auto &f : forces) {
  //   printf("%e\n", f);
  // }

  return 0;
}
