#ifndef MAP_LIST_NODE_HPP
#define MAP_LIST_NODE_HPP
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include "../enums.hpp"
#include "../project_one/data_structs/map/map.hpp"

class MapListNode
{
private:
public:
  Map *map;
  MapListNode *next;
  MapListNode(char *string, MapListNode *next);
  ~MapListNode();
  void print();
};

#endif