#ifndef ICE_SPELL_H
#define ICE_SPELL_H
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include "./spell.hpp"

using namespace std;

class IceSpell : public Spell
{
protected:
  int damage_reduction;

public:
  IceSpell(string name, float price, int minimum_level, int damage, int needed_magic_power, int damage_reduction, int duration);
  ~IceSpell();
  void print();
  int get_damage_reduction();
};
#endif