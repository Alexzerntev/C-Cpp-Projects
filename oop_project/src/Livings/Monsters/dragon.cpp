#include "../../../include/Livings/Monsters/dragon.hpp"

Dragon::Dragon(int level, int healthPower, string name, int damage, int defence, int flexibility)
    : Monster(level, healthPower, name, damage, defence, flexibility)
{
}

Dragon::~Dragon()
{
}

void Dragon::print()
{
    cout << this->name << ": Dragon\n"
         << "level :" << this->level << "\nHealth Power :" << this->get_healthPower() << "\nDamage :" << this->get_damage() << "\nDefence :" << this->get_defence()
         << "\nFlexibility :" << this->get_flexibility() << endl;
}