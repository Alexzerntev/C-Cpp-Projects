#ifndef STACK_NODE_HPP
#define STACK_NODE_HPP
#include <cstdio>
#include <cstring>
#include <cstdlib>

class TrieNode;

class StackNode
{
private:
public:
  StackNode *up_node;
  StackNode *down_node;
  TrieNode *trie_node;

  StackNode();
  StackNode(TrieNode *trie_node, StackNode *up_node, StackNode *down_node);
  ~StackNode();
};

#endif