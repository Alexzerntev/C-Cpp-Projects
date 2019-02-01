#ifndef USER_INTERACTION_HPP
#define USER_INTERACTION_HPP
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <math.h>
#include "./enums.hpp"
#include "../include/string/order_string.hpp"

class UserInteraction
{
private:
public:
  UserInteraction();
  ~UserInteraction();
  bool validate(int argument_count, char *arguments[]);
  char *interact();
};

#endif