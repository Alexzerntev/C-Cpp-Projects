#ifndef ORDER_STRING_HPP
#define ORDER_STRING_HPP
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <cerrno>
#include "../enums.hpp"

class OrderString
{
private:
  orderType type;
  int deadline;

public:
  char *string;
  OrderString(char *str);
  ~OrderString();
  orderType get_type();
  void set_string(char *str);
  char *get_string();
  int get_deadline();
  char *get_word_by_index(int index);
  int get_words_count();
};

#endif