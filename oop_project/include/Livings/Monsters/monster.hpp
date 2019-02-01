#ifndef MONSTER_H
#define MONSTER_H
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <ctime>
#include "../living.hpp"
#include "../../Usables/effect.hpp"
#include "../../enums.hpp"

class Hero;

using namespace std;

class Monster : public Living
{
private:
  int damage;
  int defence;
  int flexibility;

protected:
  int get_damage();
  int get_defence();
  int get_flexibility();

public:
  Monster(int level, int healthPower, string name, int damage, int defence, int flexibility);
  ~Monster();

  void take_attack(int damage);
  void attack(Hero *hero);
  void take_spell_attack(Ability type, int amount, int duration, int damage);

  virtual void print() = 0;
};

#endif