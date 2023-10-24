#pragma once
#include <gflags/gflags.h>
#include <mpi.h>

#include "../particle.h"
#include "../queue/queue_dispatcher.h"
#include "tag.h"

DEFINE_int32(chunk_size, 10, "chunk size, obviously");

class MPIWorker {
public:
  MPIWorker(int mpi_rank, int thread_num)
      : mpi_rank_(mpi_rank), thread_num_(thread_num) {}

  void run() {
    debug_printf("MPI worker %d started\n", mpi_rank_);
    int vector_size = 0;
    MPI_Recv(&vector_size, 1, MPI_INT, 0, MPI_Tag::INT, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    debug_printf("worker %d vec size received, %d\n", mpi_rank_, vector_size);

    std::vector<Particle> particles;
    particles.resize(vector_size);

    int extra_begin = 0, extra_end = 0;
    MPI_Recv(&extra_begin, 1, MPI_INT, 0, MPI_Tag::INT, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    MPI_Recv(&extra_end, 1, MPI_INT, 0, MPI_Tag::INT, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);

    std::vector<int> particle_ints;
    particle_ints.reserve(vector_size * 2);
    MPI_Recv(particle_ints.data(), vector_size * 2, MPI_INT, 0, MPI_Tag::INT,
             MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    for (int i = 0; i < vector_size; ++i) {
      particles[i].x = particle_ints[i * 2];
      particles[i].y = particle_ints[i * 2 + 1];
    }
    debug_printf(
        "worker %d vec received, size:%lu, extra_begin:%d, extra_end:%d\n",
        mpi_rank_, particles.size(), extra_begin, extra_end);

    std::vector<double> forces;
    forces.resize(vector_size);

    using namespace std::chrono;
    auto bb = high_resolution_clock::now();
    QueueDispatcher dispatcher(mpi_rank_, thread_num_, FLAGS_chunk_size);
    int calculate_end = vector_size - 1 - extra_end;
    dispatcher.run(particles, forces, extra_begin, calculate_end);
    auto ee = high_resolution_clock::now();
    long cal_cost = duration_cast<milliseconds>(ee - bb).count();

    debug_printf("worker %d calculated, size:%d, begin:%d, end:%d\n", mpi_rank_,
                 vector_size, extra_begin, calculate_end);
    int send_back_begin = (!!extra_begin) ? 1 : 0;
    int send_back_data_size = vector_size - extra_begin - extra_end;
    debug_printf("MPI worker %d send back data, size:%d, begin:%d\n", mpi_rank_,
                 send_back_data_size, send_back_begin);
    MPI_Send(&(forces[send_back_begin]), send_back_data_size, MPI_DOUBLE, 0,
             MPI_Tag::DOUBLE, MPI_COMM_WORLD);

    MPI_Send(&cal_cost, 1, MPI_LONG, 0, MPI_Tag::LONG, MPI_COMM_WORLD);
  }

private:
  int mpi_rank_;
  int thread_num_;
};