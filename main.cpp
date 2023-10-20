#include <chrono>
#include <gflags/gflags.h>
#include <mpi.h>
#include <stdexcept>

#include "src/csv/csv_reader.h"
#include "src/even_threads/even_dispatcher.h"
#include "src/mpi/mpi_dispatcher.h"
#include "src/mpi/mpi_worker.h"
#include "src/serial/serial.h"

DEFINE_int32(mode, 0, "mode number");
DEFINE_int32(particle_num, 100, "number of particles to calculate");
DEFINE_int32(thread_num, 10, "number of threads in mode 1");

int main(int argc, char *argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  printf("particle num:%d\n", FLAGS_particle_num);

  auto particles = read_csv(FLAGS_particle_num);

  std::vector<double> forces;
  forces.resize(particles.size());

  auto start = std::chrono::high_resolution_clock::now();

  switch (FLAGS_mode) {
  case 0:
    calculate_closest(particles);
    calculate_force(particles, forces);
    break;
  case 1: {
    EvenDispatcher dispatcher(FLAGS_thread_num);
    dispatcher.run(particles, forces);
    break;
  }
  case 2: {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    // printf("size:%d, rank:%d\n", size, rank);

    if (rank == 0) {
      MPIDispatcher dispatcher(size - 1);
      dispatcher.run(particles);
    } else {
      MPIWorker worker(rank, FLAGS_thread_num);
      worker.run();
    }

    MPI_Finalize();
    break;
  }

  default:
    throw std::runtime_error("this mode is not implemented");
  }

  auto end = std::chrono::high_resolution_clock::now();

  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  printf(" Time taken by function: %ld microseconds\n", duration.count());

  for (size_t i = 0; i < forces.size() && i < 10; ++i) {
    printf("index:%lu, particle:%s, force:%e\n", i,
           particles[i].to_string().c_str(), forces[i]);
  }

  return 0;
}
