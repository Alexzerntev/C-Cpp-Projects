#ifndef SEARCH_RESULT_HPP
#define SEARCH_RESULT_HPP
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "../../string/document_string.hpp"
#include "../../string/command_string.hpp"

class SearchResult
{
private:
  DocumentString *document_string;
  float score;
  char *word;

public:
  SearchResult();
  SearchResult(DocumentString *document_string, char *word, float score);
  ~SearchResult();

  float get_score();
  int get_digits(int number);

  int get_id();

  void print(CommandString *command_string);
};

#endif