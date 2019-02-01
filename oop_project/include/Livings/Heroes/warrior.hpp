#ifndef WARRIOR_H
#define WARRIOR_H
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include "../Heroes/hero.hpp"
#include "../../enums.hpp"
#include "../../Usables/Inventory/inventory.hpp"

using namespace std;

class Warrior : public Hero
{
private:
public:
  Warrior(int level, int helthPower, string name,
          int magicPower, int strength, int dexterity,
          int agility, int money, int experience,
          HeroType heroType);
  ~Warrior();
  void print();
  int get_level();
};

#endif