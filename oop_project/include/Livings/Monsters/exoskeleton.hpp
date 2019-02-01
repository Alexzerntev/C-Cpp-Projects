#ifndef EXOSKELETON_H
#define EXOSKELETON_H
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include "../Monsters/monster.hpp"

using namespace std;

class Exoskeleton : public Monster
{
private:
public:
  Exoskeleton(int level, int helthPower, string name, int damage, int defence, int flexibility);
  ~Exoskeleton();
  void print();
};

#endif