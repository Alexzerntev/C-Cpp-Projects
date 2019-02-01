#ifndef MARKET_SQUARE_H
#define MARKET_SQUARE_H
#include <iostream>
#include <string>
#include <vector>
#include "../../include/enums.hpp"
#include "../enums.hpp"
#include "../Usables/Items/armor.hpp"
#include "../Usables/Items/weapon.hpp"
#include "../Usables/Items/potion.hpp"
#include "../Usables/Spells/spell.hpp"
#include "../Usables/Spells/fire_spell.hpp"
#include "../Usables/Spells/ice_spell.hpp"
#include "../Usables/Spells/lightnig_spell.hpp"
#include "../Livings/living.hpp"
#include "../Grid/square.hpp"
#include "../Livings/Monsters/dragon.hpp"
#include "../Livings/Monsters/exoskeleton.hpp"
#include "../Livings/Monsters/spirit.hpp"

using namespace std;

class MarketSquare : public Square
{
private:
  vector<Weapon *> weapons;
  vector<Armor *> armors;
  vector<Spell *> spells;
  vector<Potion *> potions;
  void add_to_weapons(Weapon *weapon);
  void add_to_armors(Armor *armor);
  void add_to_spells(Spell *spell);
  void add_to_potions(Potion *potion);

public:
  MarketSquare();
  MarketSquare(int market_index);
  void add_usable(Usable* usable);

  void print_weapons();
  void print_armors();
  void print_spells();
  void print_potions();

  int get_collection_size(UsableType usableType);
  Usable *get_item_by_index(int index, UsableType usableType);
  void erase_item_by_index(int index, UsableType usableType);

  ~MarketSquare();
};
#endif