#ifndef SPELL_H
#define SPELL_H
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include "../usable.hpp"

using namespace std;

class Spell : public Usable
{
protected:
  int damage;
  int needed_magic_power;
  int duration;
  Spell(string name, float price, int minimum_level, int damage, int needed_magic_power, int duration);
  ~Spell();

public:
  int get_needed_magic_power();
  int get_duration();
  int get_damage();
  virtual void print() = 0;
};
#endif