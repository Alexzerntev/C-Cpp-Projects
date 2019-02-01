#ifndef FIRE_SPELL_H
#define FIRE_SPELL_H
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include "./spell.hpp"

using namespace std;

class FireSpell : public Spell
{
protected:
  int defence_reduction;

public:
  FireSpell(string name, float price, int minimum_level, int damage, int needed_magic_power, int defence_reduction, int duration);
  ~FireSpell();
  void print();
  int get_defence_reduction();
};
#endif