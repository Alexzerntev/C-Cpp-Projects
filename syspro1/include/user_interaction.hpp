#ifndef USER_INTERACTION_HPP
#define USER_INTERACTION_HPP
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <math.h>
#include "./string/command_string.hpp"
#include "./enums.hpp"
#include "./data_structures/map/map.hpp"
#include "../include/data_structures/search_result/search_result.hpp"
#include "../include/data_structures/heap/heap.hpp"

const float k = 1.2;
const float b = 0.75;

class UserInteraction
{
private:
  int result_count;
  Map *map;
  float get_score(CommandString *command_string, char *word, int id);
  float get_idf(CommandString *command_string);
  float get_coefficient(char *word, int id);
  void search(CommandString *command_string);

public:
  UserInteraction();
  ~UserInteraction();
  bool validate(int argument_count, char *arguments[]);
  result interact();
  void set_map(Map *map);
};

#endif