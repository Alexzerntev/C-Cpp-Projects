#ifndef POTION_H
#define POTION_H
#include <iostream>
#include <string>
#include <vector>
#include "./item.hpp"
#include "../../enums.hpp"

using namespace std;

class Potion : public Item
{
protected:
  Ability ability;
  int duration;
  unsigned int amount;

public:
  Potion(string name, float price, int minimum_level, Ability ability, unsigned int amount, int duration);
  ~Potion();
  Ability get_type();
  int get_amount();
  int get_duration();

  void print();
};
#endif