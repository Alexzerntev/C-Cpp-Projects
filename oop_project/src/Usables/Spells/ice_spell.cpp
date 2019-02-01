#include "../../../include/Usables/Spells/ice_spell.hpp"
#include "../../../include/Usables/Spells/spell.hpp"

using namespace std;

IceSpell::IceSpell(string name, float price, int minimum_level,
                   int damage, int needed_magic_power, int damage_reduction, int duration)
    : Spell(name, price, minimum_level, damage, needed_magic_power, duration)
{
    this->damage_reduction = damage_reduction;
}
IceSpell::~IceSpell() {}

void IceSpell::print()
{
    cout << this->name << ": Ice Spell\n"
         << "Price: " << this->price << "\nDamage: " << this->damage << "\nminimum level: " << this->minimum_level
         << "\nneed magic power: " << this->needed_magic_power << "\nDamage redution: " << this->damage_reduction
         << "\nDuration: " << this->duration << endl;
    ;
}

int IceSpell::get_damage_reduction()
{
    return this->damage_reduction;
}
