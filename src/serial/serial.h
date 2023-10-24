#pragma once
#include <cmath>
#include <vector>

#include "../particle.h"

const long coulombK = 8.99e9;
// const double charge = 1.6 * std::pow(10, -19);
const double charge = 1.6e-19;
const double power_charge = std::pow(charge,2);
// const double unit_size = std::pow(10, -10);
const double unit_size = 1e-10;

void calculate_closest(std::vector<Particle> &particles, int begin = -1,
                       int end = -1);

void calculate_force(const std::vector<Particle> &particles,
                     std::vector<double> &results, int begin = -1,
                     int end = -1);
