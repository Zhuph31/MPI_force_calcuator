#pragma once

#include <memory>
#include <mpi.h>

#include "../even_threads/even_dispatcher.h"
#include "../particle.h"

class MPIDispatcher {
public:
  MPIDispatcher(int process_num) : process_num_(process_num) {}

  void run(std::vector<Particle> &particles) {

    // int rank, size;
    // MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // MPI_Comm_size(MPI_COMM_WORLD, &size);

    // if (rank == 0) {
    //   for (int i = 1; i < size; i++) {
    //     // 发送任务到其他进程
    //     MPI_Send(&i, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
    //   }
    // } else {
    //   // 从其他进程接收任务
    //   int task;
    //   MPI_Recv(&task, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    //   // 执行任务
    // }
  }

  void calculate() {}

private:
  int process_num_;
  std::unique_ptr<EvenDispatcher> dispatcher_;
};