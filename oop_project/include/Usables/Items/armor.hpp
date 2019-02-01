#ifndef ARMOR_H
#define ARMOR_H
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include "./item.hpp"

using namespace std;

class Armor : public Item
{
protected:
  int defence;

public:
  Armor();
  Armor(string name, float price, int minimum_level, int defence);
  ~Armor();
  void print();
  int get_defence();
};
#endif