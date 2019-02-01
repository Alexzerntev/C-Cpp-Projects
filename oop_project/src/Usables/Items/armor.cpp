#include "../../../include/Usables/Items/item.hpp"
#include "../../../include/Usables/Items/armor.hpp"

using namespace std;

Armor::Armor() : Item(name, price, minimum_level) {}

Armor::Armor(string name, float price, int minimum_level, int defence)
    : Item(name, price, minimum_level)
{
    this->defence = defence;
}
Armor::~Armor() {}

int Armor::get_defence()
{
    return this->defence;
}

void Armor::print()
{
    cout << this->name << ": Armor\n"
         << "Price: " << this->price << "\nDefence: " << this->defence << "\nminimum level: " << this->minimum_level << endl;
}
