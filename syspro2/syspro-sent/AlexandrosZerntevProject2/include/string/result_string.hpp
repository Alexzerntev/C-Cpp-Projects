#ifndef RESULT_STRING_HPP
#define RESULT_STRING_HPP
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <cerrno>
#include "../enums.hpp"

class ResultString
{
  private:
    orderType type;

  public:
    char *string;
    int minimax;
    unsigned long lines_count;
    unsigned long words_count;
    unsigned long bytes_count;
    ResultString *next;
    ResultString(char *str, ResultString *next, orderType type);
    ~ResultString();
    orderType get_type();
    void set_data();
};

#endif