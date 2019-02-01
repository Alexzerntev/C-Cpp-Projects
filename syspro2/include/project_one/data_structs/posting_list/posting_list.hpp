#ifndef PL_LIST_HPP
#define PL_LIST_HPP
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "../../../enums.hpp"
#include "../posting_list/posting_list_node.hpp"

class PostingList
{
  private:
    int count;

  public:
    PostingList();
    ~PostingList();
    PostingListNode *posting_list_next_node;
    void increase_count();
    int get_count();
};

#endif