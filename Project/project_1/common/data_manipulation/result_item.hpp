#ifndef RESULT_ITEM_HPP
#define RESULT_ITEM_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "../enums.hpp"

using namespace std;

class ResultItem
{
private:
public:
  unsigned long int id;
  double distance;
  ResultItem();
  ResultItem(unsigned long int id, double distance);
};

bool operator<(const ResultItem &r1, const ResultItem &r2);
bool compareResult(ResultItem *a, ResultItem *b);

#endif
