#include "../../../include/Usables/Items/item.hpp"
#include "../../../include/Usables/Items/weapon.hpp"

using namespace std;

Weapon::Weapon() : Item(name, price, minimum_level) {}

Weapon::Weapon(string name, float price, int minimum_level, int damage, bool is_two_handed)
    : Item(name, price, minimum_level)
{
    this->damage = damage;
    this->is_two_handed = is_two_handed;
}
Weapon::~Weapon() {}

int Weapon::get_damage()
{
    return this->damage;
}

bool Weapon::get_is_two_handed()
{
    return this->is_two_handed;
}

void Weapon::print()
{
    string ore;
    if (this->is_two_handed == 0)
        ore = "no";
    else
        ore = "yes";

    cout << this->name << ": Weapon\n"
         << "Price: " << this->price << "\nDamage: " << this->damage << "\nminimum level: " << this->minimum_level
         << "\nis two handed: " << ore << endl;
}
