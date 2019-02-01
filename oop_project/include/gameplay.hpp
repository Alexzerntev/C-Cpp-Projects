#ifndef GAMEPLAY_H
#define GAMEPLAY_H
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "../include/Grid/grid.hpp"
#include "../include/Livings/Heroes/hero.hpp"
#include "../include/Livings/Heroes/paladin.hpp"
#include "../include/Livings/Heroes/sorcerer.hpp"
#include "../include/Livings/Heroes/warrior.hpp"
#include "../include/helpers.hpp"
#include "../include/enums.hpp"
#include "../include/Livings/Monsters/spirit.hpp"
#include "../include/Livings/Monsters/exoskeleton.hpp"
#include "../include/Livings/Monsters/dragon.hpp"
#include "../include/Livings/Monsters/monster.hpp"

using namespace std;

class Gameplay
{
private:
  int x;
  int y;
  vector<Hero *> heroes;
  Grid gamplayGrid;
  void CheckInventory();
  void Move();
  void Fight(CommonSquare *coSq);
  void GoToTheMarket(MarketSquare *maSq);
  Monster *get_random_Monster(Hero* hero);


public:
  Gameplay();
  int GetHeroesX();
  int GetHeroesY();

  int ShowOptions();
  void ShowStatistics();

  ~Gameplay();
};

#endif