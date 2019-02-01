#ifndef INITIALIZE_CUBE_HPP
#define INITIALIZE_CUBE_HPP

#include <iostream>
#include <string>
#include "../../common/initialize_base.hpp"

using namespace std;

class InitializeCube : public InitializeBase
{
private:
  // locality sensitive count
  int k_;
  // hash table count
  unsigned int M_;
  int probes_;
  void user_interact(AttributeType type);

public:
  InitializeCube(int argc, char **argv);
  void manual_init(bool &is_first_time);
  int get_k();
  unsigned int get_M();
  int get_probes();
};

#endif