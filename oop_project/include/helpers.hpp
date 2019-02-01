#ifndef HELPERS_H
#define HELPERS_H
#include <iostream>
#include <string>
#include <vector>
#include "../include/enums.hpp"
#include "../include/Livings/Monsters/monster.hpp"
#include "../include/Usables/Spells/spell.hpp"
#include "../include/Livings/Heroes/paladin.hpp"
#include "../include/Livings/Heroes/sorcerer.hpp"
#include "../include/Livings/Heroes/warrior.hpp"
#include "../include/data.hpp"
#include "../include/enums.hpp"
#include "../include/Usables/Items/potion.hpp"
#include "../include/Usables/Items/item.hpp"

int pure_text_menu(MenuType type);
int number_validation(int size);
Monster *monsters_menu(vector<Monster *> &monster);
Spell *spells_menu(vector<Spell *> &spells);
Potion *potion_menu(vector<Potion *> &potions);
Item *equipable_items_menu(vector<Item *> &items);
Hero *heroes_menu(vector<Hero *> &heroes);

#endif
