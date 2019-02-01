#ifndef LIGHTNING_SPELL_H
#define LIGHTNING_SPELL_H
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include "./spell.hpp"

using namespace std;

class LightningSpell : public Spell
{
protected:
  int dodge_reduction;

public:
  LightningSpell(string name, float price, int minimum_level, int damage, int needed_magic_power,
                 int dodge_reduction, int duration);
  ~LightningSpell();
  void print();
  int get_dodge_reduction();
};
#endif