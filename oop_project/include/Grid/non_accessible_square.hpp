#ifndef NON_ACCESSIBLE_SQUARE_H
#define NON_ACCESSIBLE_SQUARE_H
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include "../Grid/square.hpp"
#include "../enums.hpp"

using namespace std;

class NonAccesableSquare : public Square
{
  private:
    
  public:
    NonAccesableSquare();
    ~NonAccesableSquare();
};
#endif