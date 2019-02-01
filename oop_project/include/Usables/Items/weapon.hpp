#ifndef WEAPON_H
#define WEAPON_H
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include "./item.hpp"

using namespace std;

class Weapon : public Item
{
private:
  int damage;
  bool is_two_handed;

public:
  Weapon();
  Weapon(string name, float price, int minimum_level, int damage, bool is_two_handed);
  ~Weapon();
  void print();
  int get_damage();
  bool get_is_two_handed();
};
#endif