#ifndef LIVING_H
#define LIVING_H
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include "../Usables/Inventory/inventory.hpp"
#include "../enums.hpp"
#include "../Usables/effect.hpp"

using namespace std;

class Living
{
  private:
	int healthPower;

  protected:
	string name;
	int level;
	int max_healthPower;
	vector<Effect> effects;

  public:
	Living(int level, int healthPower, string name);
	~Living();

	bool is_stunned();

	string get_name();
	int get_healthPower();
	void set_healthPower(int hp);
	void pass_effect();

	virtual void print() = 0;
};

#endif