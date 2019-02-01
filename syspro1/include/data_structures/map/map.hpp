#ifndef MAP_HPP
#define MAP_HPP
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "../../enums.hpp"
#include "../../string/document_string.hpp"
#include "../trie/trie_node.hpp"

class Map
{
private:
  char *filename;
  unsigned int entry_count;

  double avgdl;

  TrieNode *trie;

  result find_lines_count();
  result load_lines();

public:
  Map(char *filename);
  ~Map();

  DocumentString **entries;
  int get_entry_count();
  double get_entry_avgdl();
  void print_strings();
  //void print_trie();
  int get_word_frequancy(char *word);
  int get_word_document_frequancy(char *word, int id);
  PostingList *get_words_posting_list(char *word);
  void print_all_trie_frequancy();

  result execute_mapping();
};

#endif