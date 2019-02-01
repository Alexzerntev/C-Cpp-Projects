#ifndef HEAP_HPP
#define HEAP_HPP
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "../search_result/search_result.hpp"

class Heap
{
private:
  SearchResult **heap_table;
  CommandString *command_string;
  int size;
  int current_size;
  void reheap(int i);
  void swap(SearchResult **a, SearchResult **b);
  SearchResult *get_max_after_reheap();
  void max_heapify();

public:
  Heap(int size, CommandString *command_string);
  ~Heap();
  void add(SearchResult *search_result);
  void print_results();
  bool id_exists(int id);
};

#endif