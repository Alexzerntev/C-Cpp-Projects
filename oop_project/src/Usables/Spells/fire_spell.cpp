#include "../../../include/Usables/Spells/fire_spell.hpp"
#include "../../../include/Usables/Spells/spell.hpp"

using namespace std;

FireSpell::FireSpell(string name, float price, int minimum_level,
                     int damage, int needed_magic_power, int defence_reduction, int duration)
    : Spell(name, price, minimum_level, damage, needed_magic_power, duration)
{
    this->defence_reduction = defence_reduction;
}
FireSpell::~FireSpell() {}

void FireSpell::print()
{
    cout << this->name << ": Fire Spell\n"
         << "Price: " << this->price << "\nDamage: " << this->damage << "\nminimum level: " << this->minimum_level
         << "\nneed magic power :" << this->needed_magic_power << "\nDefence redution: " << this->defence_reduction
         << "\nDuration: " << this->duration << endl;
}

int FireSpell::get_defence_reduction()
{
    return this->defence_reduction; 
}
