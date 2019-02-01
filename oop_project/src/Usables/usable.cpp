#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "../../include/Usables/usable.hpp"

using namespace std;

Usable::Usable(string name, float price, int minimum_level)
{
    this->minimum_level = minimum_level;
    this->price = price;
    this->name = name;
}

float Usable::get_price()
{
    return this->price;
}

int Usable::get_minimum_level()
{
    return this->minimum_level;
}

Usable::~Usable() {}
