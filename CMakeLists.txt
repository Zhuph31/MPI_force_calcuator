cmake_minimum_required(VERSION 3.0)

project(force_calculator)

find_package(gflags REQUIRED)
find_package(MPI REQUIRED)
find_package(OpenMP REQUIRED)
add_definitions(-DOMPI_SKIP_MPICXX)

# Find MPI
find_package(MPI REQUIRED)
# Include MPI directories
include_directories(${MPI_INCLUDE_PATH})

# Link against MPI libraries

list(APPEND CMAKE_MODULE_PATH "/usr/lib/x86_64-linux-gnu/openmpi/lib")

include_directories(${gflags_INCLUDE_DIR})
include_directories(SYSTEM "/usr/lib/x86_64-linux-gnu/openmpi/include")

link_directories(SYSTEM "/usr/lib/x86_64-linux-gnu/openmpi/lib")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")
add_executable(force_calculator main.cpp ${SOURCES})

add_compile_options(-Wno-sign-compare)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

target_link_libraries(force_calculator PUBLIC gflags OpenMP::OpenMP_CXX ${MPI_LIBRARIES})

