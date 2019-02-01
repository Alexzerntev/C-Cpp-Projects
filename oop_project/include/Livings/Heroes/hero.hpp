#ifndef HERO_H
#define HERO_H
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <ctime>
#include "../living.hpp"
#include "../../Usables/effect.hpp"
#include "../../enums.hpp"
#include "../Monsters/monster.hpp"

using namespace std;

class Hero : public Living
{
private:
  int magicPower;
  int max_MagicPower;
  int strength;
  int dexterity;
  int agility;

protected:
  void level_up();
  int get_damage();
  int get_magicPower();
  int get_strength();
  int get_dexterity();
  int get_agility();
  int money;
  int experience;
  int experience_to_next_level;
  Inventory *invertory;
  Armor *armor;
  Weapon *weapon[2];

public:
  Hero(int level, int healthPower, string name,
       int magicPower, int strength, int dexterity,
       int agility, int money,
       int experience, HeroType heroType);
  ~Hero();

  int get_inventory_size();
  Usable *sell_item(int index);
  Usable *get_inventory_items(int index);
  int buy_item(Usable *usable);

  void attack(Monster *monster);
  void take_attack(int damage);
  Spell *choose_spell_to_cast();
  void cast_spell(Monster *monster, Spell *spell);
  Potion *choose_potion_to_drink();
  void drink_potion(Potion *potion);
  Item *choose_item_to_equip();
  void equip_item(Item *item);
  void after_fight_win(int monsters_killed);
  void after_fight_lose();

  void print_inventory();
  void print_invertory_wepons();
  void print_inverory_armors();
  virtual void print() = 0;
  virtual int get_level() = 0;
};

#endif