#ifndef GRID_H
#define GRID_H
#include <iostream>
#include <string>
#include <vector>
#include "./square.hpp"
#include "./market_square.hpp"
#include "./non_accessible_square.hpp"
#include "./common_square.hpp"

using namespace std;

class Grid
{
private:
public:
  Square *grid[12][12];
  Grid();
  ~Grid();
  void DisplayMap(int x, int y);
};
#endif