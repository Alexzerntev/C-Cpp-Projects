#ifndef DOCUMENT_STRING_HPP
#define DOCUMENT_STRING_HPP
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "./string.hpp"

class DocumentString : public String
{
private:
  int id;

public:
  DocumentString(char *string);
  ~DocumentString();
  int get_id();
  int get_frequancy(char *word);
};

#endif