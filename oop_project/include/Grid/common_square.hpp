#ifndef COMMON_SQUARE_H
#define COMMON_SQUARE_H
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include "../../include/enums.hpp"
#include "../Livings/Monsters/monster.hpp"
#include "../Grid/square.hpp"

using namespace std;

class CommonSquare : public Square
{
private:
public:
  vector<Monster *> monsters_on_squere;
  CommonSquare();
  CommonSquare(Monster *monster);
  ~CommonSquare();
  void print_monsters();
};
#endif