#ifndef PATH_LIST_NODE_HPP
#define PATH_LIST_NODE_HPP
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include "../enums.hpp"
#include "../project_one/data_structs/map/map.hpp"

class PathListNode
{
  private:
  public:
    char *string;
    PathListNode *next;
    PathListNode(char *string, PathListNode *next);
    ~PathListNode();
    void print();
};

#endif