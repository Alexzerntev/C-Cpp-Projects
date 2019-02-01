#include "../../../include/Usables/Spells/spell.hpp"
#include "../../../include/Usables/usable.hpp"

using namespace std;

Spell::Spell(string name, float price, int minimum_level, int damage, int needed_magic_power, int duration)
    : Usable(name, price, minimum_level)
{
    this->damage = damage;
    this->needed_magic_power = needed_magic_power;
    this->duration = duration;
}
Spell::~Spell() {}

int Spell::get_needed_magic_power()
{
    return this->needed_magic_power;
}

int Spell::get_duration()
{
    return this->duration;
}

int Spell::get_damage()
{
    return this->damage;
}
