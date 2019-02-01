#ifndef INITIALIZE_BASE_HPP
#define INITIALIZE_BASE_HPP

#include <string>
#include <iostream>
#include "./enums.hpp"

using namespace std;

class InitializeBase
{

private:
protected:
  string input_file_;
  string query_file_;
  string output_file_;
  bool is_valid_;
  void user_interact(AttributeType type);
  InitializeBase(int argc, char *argv[]);

public:
  string get_input_file();
  string get_query_file();
  string get_output_file();
  bool is_valid();
  bool should_repeat();
  void manual_init();
};

#endif

// InitializeBase(/* string input_file, string query_file, string output_file */);
//     //: input_file_(input_file), query_file_(query_file), output_file_(output_file) {}