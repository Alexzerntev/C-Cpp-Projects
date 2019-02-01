#ifndef DOCUMENT_STRING_HPP
#define DOCUMENT_STRING_HPP
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "./string.hpp"

class DocumentString : public String
{
private:
  int index;

public:
  DocumentString(char *string, int index);
  ~DocumentString();
  int get_index();
  int get_frequancy(char *word);
};

#endif