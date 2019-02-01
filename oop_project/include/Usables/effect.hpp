#ifndef SPELL_EFFECT_H
#define SPELL_EFFECT_H
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include "../enums.hpp"

using namespace std;

class Effect
{
private:
  int rounds;
  int amount;
  Ability type;

public:
  int get_rounds();
  int get_amount();
  void reduce_duration();
  bool is_passed();
  Ability get_type();
  Effect(int rounds, int amount, Ability type);
  ~Effect();
};

#endif