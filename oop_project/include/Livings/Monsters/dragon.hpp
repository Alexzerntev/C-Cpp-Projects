#ifndef DRAGON_H
#define DRAGON_H
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include "../Monsters/monster.hpp"

using namespace std;

class Dragon : public Monster
{
private:
public:
  Dragon(int level, int helthPower, string name, int damage, int defence, int flexibility);
  ~Dragon();
  void print();
};

#endif