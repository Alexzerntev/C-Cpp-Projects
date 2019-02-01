#ifndef SPIRIT_H
#define SPIRIT_H
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include "../Monsters/monster.hpp"

using namespace std;

class Spirit : public Monster
{
private:
public:
  Spirit(int level, int helthPower, string name, int damage, int defence, int flexibility);
  ~Spirit();
  void print();
};

#endif