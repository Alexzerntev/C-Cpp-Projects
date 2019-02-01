#include "../../../include/Livings/Monsters/spirit.hpp"

Spirit::Spirit(int level, int healthPower, string name, int damage, int defence, int flexibility) : Monster(level, healthPower, name, damage, defence, flexibility)
{
}

Spirit::~Spirit()
{
}
void Spirit::print()
{
    cout << this->name << ": Spirit\n"
         << "level :" << this->level << "\nHealth Power :" << this->get_healthPower() << "\nDamage :" << this->get_damage() << "\nDefence :" << this->get_defence()
         << "\nFlexibility :" << this->get_flexibility() << endl;
}
