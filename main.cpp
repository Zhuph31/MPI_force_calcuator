#include <chrono>
#include <gflags/gflags.h>
#include <mpi.h>
#include <stdexcept>
#include <unistd.h>

#include "src/csv/csv_reader.h"
#include "src/even_threads/even_dispatcher.h"
#include "src/mpi/mpi_dispatcher.h"
#include "src/mpi/mpi_worker.h"
#include "src/serial/serial.h"

DEFINE_bool(show_result, false, "");
DEFINE_bool(perf, false, "");
DEFINE_int32(mode, 1, "mode number");
DEFINE_int32(particle_num, 100, "number of particles to calculate");
DEFINE_int32(thread_num, 10, "number of threads in mode 1");

int main(int argc, char *argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  // debug_printf("particle num:%d\n", FLAGS_particle_num);
  using namespace std::chrono;

  int rank = -1, size = -1;

  if (FLAGS_mode == 3) {
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
  }

  std::vector<Particle> particles;
  if (FLAGS_mode == 3 && rank != 0) {
  } else {
    auto csv_bb = high_resolution_clock::now();
    particles = read_csv(FLAGS_particle_num);
    printf("read csv cost:%ld\n",
           duration_cast<milliseconds>(high_resolution_clock::now() - csv_bb)
               .count());
  }

  std::vector<double> forces;
  forces.resize(particles.size());

  auto start = std::chrono::high_resolution_clock::now();

  if (FLAGS_perf) {
    for (int thread = 1; thread <= 32; ++thread) {
      // printf("%d threads:\n", thread);
      EvenDispatcher dispatcher(thread);
      dispatcher.run(particles, forces);
      // printf("\n");
    }
    return 0;
  }

  switch (FLAGS_mode) {
  case 1:
    calculate_closest(particles);
    calculate_force(particles, forces);
    break;
  case 2: {
    EvenDispatcher dispatcher(FLAGS_thread_num);
    dispatcher.run(particles, forces);
    break;
  }
  case 3: {
    // debug_printf("size:%d, rank:%d\n", size, rank);
    debug_printf("process %d rank %d\n", getpid(), rank);

    if (rank == 0) {
      MPIDispatcher dispatcher(size - 1);
      dispatcher.run(particles, forces);
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

  if (FLAGS_mode != 2 || rank == 0) {
    auto end = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    debug_printf(" Time taken by function: %ld microseconds\n",
                 duration.count());

    // for (size_t i = 0; i < forces.size(); ++i) {
    //   if (FLAGS_show_result) {
    //     printf("index:%lu, particle:%s, force:%e\n", i + 1,
    //            particles[i].to_string().c_str(), forces[i]);
    //   } else {
    //     debug_printf("index:%lu, particle:%s, force:%e\n", i + 1,
    //                  particles[i].to_string().c_str(), forces[i]);
    //   }
    // }
  }

  return 0;
}
