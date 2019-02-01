#ifndef PALADIN_H
#define PALADIN_H
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include "../Heroes/hero.hpp"

using namespace std;

class Paladin : public Hero
{
private:
public:
  Paladin(int level, int healthPower, string name,
          int magicPower, int strength, int dexterity,
          int agility, int money,
          int experience, HeroType heroType);
  ~Paladin();
  void print();
  int get_level();
};

#endif