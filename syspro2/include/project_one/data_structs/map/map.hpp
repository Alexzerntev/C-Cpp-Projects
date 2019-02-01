#ifndef MAP_HPP
#define MAP_HPP
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <unistd.h>
#include "../../../enums.hpp"
#include "../../string/document_string.hpp"
#include "../../../../include/project_one/data_structs/trie/trie_node.hpp"

class Map
{
private:
  unsigned int entry_count;
  unsigned int word_count;
  unsigned long bytes_count;

  double avgdl;

  TrieNode *trie;

  result find_lines_count();
  result load_lines();

public:
  Map(char *filename);
  ~Map();
  char *filename;
  DocumentString **entries;
  int get_entry_count();
  int get_words_count();
  int get_bytes_count();
  double get_entry_avgdl();
  void print_strings();
  //void print_trie();
  int get_word_frequancy(char *word);
  int get_word_document_frequancy(char *word, int id);
  PostingList *get_words_posting_list(char *word);

  result execute_mapping();
};

#endif