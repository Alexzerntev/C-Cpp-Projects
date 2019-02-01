#include "../../../include/Livings/Monsters/exoskeleton.hpp"

Exoskeleton::Exoskeleton(int level, int healthPower, string name, int damage, int defence, int flexibility) : Monster(level, healthPower, name, damage, defence, flexibility)
{
}

Exoskeleton::~Exoskeleton()
{
}
void Exoskeleton::print()
{
    cout << this->name << ": Exoskeleton\n"
         << "level :" << this->level << "\nHealth Power :" << this->get_healthPower() << "\nDamage :" << this->get_damage() << "\nDefence :" << this->get_defence()
         << "\nFlexibility :" << this->get_damage() << endl;
}
