#ifndef USABLE_H
#define USABLE_H
#include <iostream>
#include <string>
#include <vector>
#include <ctime>

using namespace std;

class Usable
{
protected:
  string name;
  float price;
  int minimum_level;
  Usable(string name, float price, int minimum_level);
  ~Usable();

public:
  virtual void print() = 0;
  float get_price();
  int get_minimum_level();
};
#endif