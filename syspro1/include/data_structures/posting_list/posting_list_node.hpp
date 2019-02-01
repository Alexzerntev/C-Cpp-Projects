#ifndef PL_NODE_HPP
#define PL_NODE_HPP
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "../../enums.hpp"

class PostingListNode
{
private:
  int id;
  int frequency;

public:
  PostingListNode *next;
  PostingListNode(int id, int frequency);
  ~PostingListNode();

  int get_id();
  int get_frequency();
};

#endif