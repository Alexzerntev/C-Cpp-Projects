#ifndef INVENTORY_H
#define INVENTORY_H
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include "../Items/item.hpp"
#include "../Items/armor.hpp"
#include "../Items/weapon.hpp"
#include "../Items/potion.hpp"
#include "../Spells/spell.hpp"
#include "../usable.hpp"
#include "../../enums.hpp"
#include "../Spells/fire_spell.hpp"
#include "../Spells/ice_spell.hpp"
#include "../Spells/lightnig_spell.hpp"
#include "../Spells/spell.hpp"

using namespace std;

class Inventory
{
private:
  vector<Usable *> inventory_items;

public:
  Inventory();
  Inventory(HeroType heroType);
  ~Inventory();
  int get_size();
  Usable *remove_from_inventory(int index);
  Usable *get_item(int index);
  void add_item(Usable *usable);

  vector<Item *> get_equipable_items();
  vector<Potion *> get_potions();
  vector<Spell *> get_spells();
  void remove_potion(Potion *potion);
  void remove_item(Item *item);

  void print();
  void print_weapon();
  void print_armor();
};
#endif