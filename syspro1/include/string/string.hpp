#ifndef STRING_HPP
#define STRING_HPP
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include "../enums.hpp"

class String
{
private:
protected:
  String(char *string);
  String();
  ~String();

  void normalize(char *string);
  void set_string(char *string);

public:
  char *string;
  int get_length();
  int get_words_count();
  char *get_word_by_index(int index);
  void print();
};

#endif