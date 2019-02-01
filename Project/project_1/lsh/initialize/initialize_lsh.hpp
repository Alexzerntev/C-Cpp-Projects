#ifndef INITIALIZE_LSH_HPP
#define INITIALIZE_LSH_HPP

#include <iostream>
#include <string>
#include "../../common/initialize_base.hpp"

using namespace std;

class InitializeLsh : public InitializeBase
{
private:
  // locality sensitive count
  int k_;
  // hash table count
  int L_;
  void user_interact(AttributeType type);

public:
  InitializeLsh(int argc, char **argv);
  void manual_init(bool &is_first_time);
  int get_k();
  int get_L();
};

#endif