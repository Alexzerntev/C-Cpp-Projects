#ifndef TRIE_NODE_HPP
#define TRIE_NODE_HPP
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "../../../enums.hpp"
#include "../posting_list/posting_list.hpp"
#include "../posting_list/posting_list_node.hpp"
#include <cerrno>

class TrieNode
{
private:
  char content;
  bool id_exists(PostingList *posting_list, int id);

public:
  TrieNode *next_horizontal;
  TrieNode *next_vertical;
  PostingList *posting_list;
  TrieNode(char c = -1, PostingList *posting_list = NULL);
  ~TrieNode();
  char get_content();
  void set_content(char c);
  result add_word_by_char(
      char *word,
      PostingListNode *posting_list_node);
  int get_frequancy(char *word);
  PostingList *get_words_posting_list(char *word);
};

#endif