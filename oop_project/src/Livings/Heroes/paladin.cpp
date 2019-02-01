#include "../../../include/Livings/Heroes/paladin.hpp"
#include "../../../include/enums.hpp"

Paladin::Paladin(int level, int healthPower, string name,
                 int magicPower, int strength, int dexterity,
                 int agility, int money, int experience, HeroType heroType)
    : Hero(level, healthPower, name, magicPower, strength, dexterity, agility, money, experience, heroType)
{
}

Paladin::~Paladin()
{
}
void Paladin::print()
{
    cout << this->name << ": Paladin\n"
         << "level: " << this->level << "\nHealth Power: " << this->get_healthPower() << "\nMagic Power: " << this->get_magicPower() << "\nStrength: " << this->get_strength()
         << "\nDexterity: " << this->get_dexterity() << "\nAgility: " << this->get_agility() << "\nMoney: " << this->money << "\nExperience: " << this->experience << endl;
}

int Paladin::get_level()
{
    return level;
}