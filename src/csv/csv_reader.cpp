#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "csv_reader.h"

std::vector<Particle> read_csv(size_t num) {
  std::vector<Particle> particles;

  // read particles information from csv
  std::ifstream file("particles-student-1.csv");
  if (!file.is_open()) {
    std::cerr << "open csv failed" << std::endl;
    return particles;
  }

  std::string line;
  while (std::getline(file, line) && particles.size() < num) {
    std::istringstream ss(line);
    std::vector<std::string> row;
    std::string cell;

    while (std::getline(ss, cell, ',')) {
      row.push_back(cell);
    }

    assert(row.size() == 3);
    Particle p(std::stoi(row[0]), std::stoi(row[1]), row[2]);

    particles.push_back(std::move(p));
  }

  // reuse if we need more
  int pos = 0;
  while (particles.size() < num) {
    particles.push_back(particles[pos++]);
  }

  file.close();

  return particles;
}
