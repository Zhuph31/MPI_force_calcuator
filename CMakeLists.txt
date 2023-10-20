cmake_minimum_required(VERSION 3.0)

project(force_calculator)

find_package(gflags REQUIRED)
find_package(MPI REQUIRED)

find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})

include_directories(${gflags_INCLUDE_DIR})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")
add_executable(force_calculator main.cpp ${SOURCES})

add_compile_options(-Wno-sign-compare)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

target_link_libraries(force_calculator PUBLIC gflags ${MPI_LIBRARIES})

