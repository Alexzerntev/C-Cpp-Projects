#ifndef RANDOM_GENERATOR_HPP
#define RANDOM_GENERATOR_HPP

#include <string>
#include <iostream>
#include <vector>
#include <random>

using namespace std;

class RandomGenerator
{
public:
  RandomGenerator();
  double get_normal_distribution_random();
  double get_uniform_distribution_random();
  int get_uniform_distribution_binary_random();

private:
  default_random_engine generator_;
  normal_distribution<float> normal_distribution_{0, 1};
  uniform_real_distribution<float> uniform_distribution_{0, 4};
  uniform_real_distribution<float> uniform_distribution_binary_{0, 1};
};

#endif