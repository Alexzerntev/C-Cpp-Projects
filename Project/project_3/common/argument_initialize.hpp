#ifndef INITIALIZE_HPP
#define INITIALIZE_HPP

#include <string>
#include <iostream>
#include "./enums.hpp"

using namespace std;

class ArgumentInitialize
{

private:
protected:
  string input_file_;
  string config_file_;
  string output_file_;
  MetricType metric_;
  bool validate_;

public:
  string get_input_file();
  string get_config_file();
  string get_output_file();
  bool get_validate();
  MetricType get_metric();
  ArgumentInitialize(int argc, char *argv[]);
};

#endif