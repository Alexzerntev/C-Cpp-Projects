#ifndef SORCERER_H
#define SORCERER_H
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include "../Heroes/hero.hpp"
#include "../../enums.hpp"

using namespace std;

class Sorcerer : public Hero
{
private:
public:
  Sorcerer(int level, int helthPower, string name,
           int magicPower, int strength, int dexterity,
           int agility, int money,
           int experience, HeroType heroType);
  ~Sorcerer();
  void print();
  int get_level();
};

#endif