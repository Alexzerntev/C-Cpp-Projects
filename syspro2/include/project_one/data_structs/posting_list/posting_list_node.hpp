#ifndef PL_NODE_HPP
#define PL_NODE_HPP
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "../../../enums.hpp"

class PostingListNode
{
private:
  int index;
  int frequency;

public:
  PostingListNode *next;
  PostingListNode(int index, int frequency);
  ~PostingListNode();

  int get_index();
  int get_frequency();
};

#endif