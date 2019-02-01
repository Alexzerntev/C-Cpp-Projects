#include "../../../include/Usables/Items/item.hpp"
#include "../../../include/Usables/Items/potion.hpp"
#include "../../../include/enums.hpp"

using namespace std;

Potion::Potion(string name, float price, int minimum_level, Ability ability, unsigned int amount, int duration)
    : Item(name, price, minimum_level)
{
    this->ability = ability;
    this->amount = amount;
    this->duration = duration;
}

Potion::~Potion() {}

Ability Potion::get_type()
{
    return this->ability;
}

int Potion::get_amount()
{
    return this->amount;
}
int Potion::get_duration()
{
    return this->duration;
}

void Potion::print()
{
    cout << this->name << ": Potion\n"
         << "Price: " << this->price << "\nAbility: " << this->ability << "\nminimum level: " << this->minimum_level
         << "\namount: " << this->amount << endl;
}
