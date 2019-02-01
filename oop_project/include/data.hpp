#ifndef DATA_H
#define DATA_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<string> get_fight_menu();
vector<string> get_two_handed_menu();
vector<string> get_inventory_menu();
vector<string> get_global_menu();
vector<string> get_market_menu();
vector<string> get_market_items_menu();
vector<string> get_binary_menu();
string get_random_monster_name();

#endif
