#ifndef COMMAND_STRING_HPP
#define COMMAND_STRING_HPP
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "./string.hpp"
#include "../enums.hpp"

class CommandString : public String
{
private:
  commandType type;

public:
  CommandString(char *string);
  ~CommandString();
  commandType get_type();
};

#endif