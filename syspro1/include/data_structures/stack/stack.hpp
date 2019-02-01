#ifndef STACK_HPP
#define STACK_HPP
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "../stack/stack_node.hpp"
#include "../trie/trie_node.hpp"

class Stack
{
private:
  StackNode *top;

public:
  Stack();
  ~Stack();
  void print();
  TrieNode *pop();
  void push(TrieNode *trie_node);
  bool is_empty();
};

#endif