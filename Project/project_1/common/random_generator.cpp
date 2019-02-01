#include "./random_generator.hpp"

RandomGenerator::RandomGenerator()
{
}

double RandomGenerator::get_normal_distribution_random()
{
    return (double)normal_distribution_(generator_);
}

double RandomGenerator::get_uniform_distribution_random()
{
    return (double)uniform_distribution_(generator_);
}

int RandomGenerator::get_uniform_distribution_binary_random()
{
    return uniform_distribution_binary_(generator_) < 0.5 ? 0 : 1;
}