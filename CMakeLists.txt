cmake_minimum_required(VERSION 3.0)

project(force_calculator)

# find dpes
find_package(gflags REQUIRED)
find_package(OpenMP REQUIRED)
add_definitions(-DOMPI_SKIP_MPICXX)

include_directories(${gflags_INCLUDE_DIR})
include_directories(SYSTEM "/usr/lib/x86_64-linux-gnu/openmpi/include")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")

# 添加可执行文件
file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")
add_executable(force_calculator main.cpp ${SOURCES})

add_compile_options(-Wno-sign-compare)


# 生成 compile_commands.json 文件
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 指定C++标准
set(CMAKE_CXX_STANDARD 11)

# 指定C++编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# 指定可执行文件的输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 链接标准库
target_link_libraries(force_calculator gflags OpenMP::OpenMP_CXX)

