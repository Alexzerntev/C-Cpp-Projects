#include "../../../include/Livings/Heroes/sorcerer.hpp"
#include "../../../include/enums.hpp"

Sorcerer::Sorcerer(int level, int healthPower, string name, int magicPower, int strength,
                   int dexterity, int agility, int money, int experience, HeroType heroType)
    : Hero(level, healthPower, name, magicPower, strength,
           dexterity, agility, money, experience, heroType)
{
}

Sorcerer::~Sorcerer()
{
}

void Sorcerer::print()
{
    cout << this->name << ": Sorcerer\n"
         << "level: " << this->level << "\nHealth Power: " << this->get_healthPower() << "\nMagic Power: " << this->get_magicPower() << "\nStrength: " << this->get_strength()
         << "\nDexterity: " << this->get_dexterity() << "\nAgility: " << this->get_agility() << "\nMoney: " << this->money << "\nExperience: " << this->experience << endl;
}

int Sorcerer::get_level()
{
    return level;
}