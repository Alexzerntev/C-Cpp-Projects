#include "../../../include/Usables/Items/item.hpp"
#include "../../../include/Usables/usable.hpp"

using namespace std;

Item::Item(string name, float price, int minimum_level)
    : Usable(name, price, minimum_level)
{
}
Item::~Item() {}