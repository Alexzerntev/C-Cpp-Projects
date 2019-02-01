#ifndef ITEM_H
#define ITEM_H
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include "../usable.hpp"

using namespace std;

class Item : public Usable
{
protected:
  Item(string name, float price, int minimum_level);
  ~Item();

public:
 virtual void print() = 0;
};
#endif