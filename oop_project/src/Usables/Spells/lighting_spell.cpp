#include "../../../include/Usables/Spells/lightnig_spell.hpp"
#include "../../../include/Usables/Spells/spell.hpp"

using namespace std;

LightningSpell::LightningSpell(string name, float price, int minimum_level,
                               int damage, int needed_magic_power, int dodge_reduction, int duration)
    : Spell(name, price, minimum_level, damage, needed_magic_power, duration)
{
    this->dodge_reduction = dodge_reduction;
}
LightningSpell::~LightningSpell() {}

void LightningSpell::print()
{
    cout << this->name << ": Light Spell\n"
         << "Price: " << this->price << "\nDamage: " << this->damage << "\nminimum level: " << this->minimum_level
         << "\nneed magic power: " << this->needed_magic_power << "\nDodge redution: " << this->dodge_reduction
         << "\nDuration: " << this->duration << endl;
}

int LightningSpell::get_dodge_reduction()
{
    return this->dodge_reduction;
}
