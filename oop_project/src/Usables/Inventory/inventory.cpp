#include "../../../include/Usables/Items/item.hpp"
#include "../../../include/Usables/Inventory/inventory.hpp"
#include "../../../include/Usables/Spells/fire_spell.hpp"
#include "../../../include/Usables/Spells/ice_spell.hpp"
#include "../../../include/Usables/Spells/lightnig_spell.hpp"
#include "../../../include/enums.hpp"
#include "../../../include/Usables/Items/armor.hpp"
#include "../../../include/Usables/Items/weapon.hpp"
#include "../../../include/Usables/Items/potion.hpp"

using namespace std;

Inventory::Inventory()
{
}
Inventory::Inventory(HeroType hero_type)
{
    switch (hero_type)
    {
        {
        case PaladinType:
            this->inventory_items.push_back(new LightningSpell("God's Wrath", 10, 1, 15, 8, 10, 3));
            this->inventory_items.push_back(new Potion("Potion Of Strength", 5, 1, Strength, 5, 3));
            break;
        case SorcererType:
            this->inventory_items.push_back(new FireSpell("Firebolt", 10, 1, 15, 8, 10, 3));
            this->inventory_items.push_back(new IceSpell("Frostbolt", 10, 1, 15, 8, 10, 3));
            this->inventory_items.push_back(new Potion("Potion Of Intelect", 5, 1, Dexterity, 5, 3));
            break;
        case WarriorType:
            this->inventory_items.push_back(new LightningSpell("Heroic Charge", 10, 1, 15, 8, 10, 3));
            this->inventory_items.push_back(new Potion("Potion Of Strength", 5, 1, Strength, 5, 3));
            break;
        }
    }
}

int Inventory::get_size()
{
    return this->inventory_items.size();
}

Usable *Inventory::remove_from_inventory(int index)
{
    Usable *itemToReturn = this->inventory_items[index];
    this->inventory_items.erase(this->inventory_items.begin() + index);
    return itemToReturn;
}

Usable *Inventory::get_item(int index)
{
    return this->inventory_items[index];
}

void Inventory::add_item(Usable *usable)
{
    this->inventory_items.push_back(usable);
}

void Inventory::print()
{

    int i = 0;
    for (i = 0; i < inventory_items.size(); i++)
    {
        if (Weapon *wpn = dynamic_cast<Weapon *>(inventory_items[i]))
        {
            cout << i + 1 << ") ";
            wpn->print();
            cout << endl;
        }
        else if (Potion *pot = dynamic_cast<Potion *>(inventory_items[i]))
        {
            cout << i + 1 << ") ";
            pot->print();
            cout << endl;
        }
        else if (Armor *armor = dynamic_cast<Armor *>(inventory_items[i]))
        {
            cout << i + 1 << ") ";
            armor->print();
            cout << endl;
        }
        else if (FireSpell *fs = dynamic_cast<FireSpell *>(inventory_items[i]))
        {
            cout << i + 1 << ") ";
            fs->print();
            cout << endl;
        }
        else if (IceSpell *is = dynamic_cast<IceSpell *>(inventory_items[i]))
        {
            cout << i + 1 << ") ";
            is->print();
            cout << endl;
        }
        else if (LightningSpell *ls = dynamic_cast<LightningSpell *>(inventory_items[i]))
        {
            cout << i + 1 << ") ";
            ls->print();
            cout << endl;
        }
    }
}

void Inventory::print_weapon()
{
    int i = 0;
    for (i = 0; i < inventory_items.size(); i++)
    {
        if (Weapon *wpn = dynamic_cast<Weapon *>(inventory_items[i]))
        {
            wpn->print();
            cout << endl;
        }
    }
}

void Inventory::print_armor()
{
    int i;
    for (i = 0; i < inventory_items.size(); i++)
    {
        if (Armor *armor = dynamic_cast<Armor *>(inventory_items[i]))
        {
            armor->print();
            cout << endl;
        }
    }
}

vector<Item *> Inventory::get_equipable_items()
{

    vector<Item *> equipable_items;

    int i = 0;
    for (i = 0; i < this->inventory_items.size(); i++)
    {
        if (Weapon *wpn = dynamic_cast<Weapon *>(this->inventory_items[i]))
        {
            equipable_items.push_back(wpn);
        }
        if (Armor *armor = dynamic_cast<Armor *>(this->inventory_items[i]))
        {
            equipable_items.push_back(armor);
        }
    }
    return equipable_items;
}

vector<Potion *> Inventory::get_potions()
{
    vector<Potion *> potions;

    int i = 0;
    for (i = 0; i < inventory_items.size(); i++)
    {
        if (Potion *poti = dynamic_cast<Potion *>(inventory_items[i]))
        {

            potions.push_back(poti);
        }
    }
    return potions;
}

vector<Spell *> Inventory::get_spells()
{
    vector<Spell *> spells;

    int i = 0;
    for (i = 0; i < inventory_items.size(); i++)
    {
        if (Spell *spell = dynamic_cast<Spell *>(inventory_items[i]))
        {

            spells.push_back(spell);
        }
    }
    return spells;
}

void Inventory::remove_potion(Potion *potion)
{
    int index_to_remove = 0;
    for (int i = 0; i < this->inventory_items.size(); i++)
    {
        if (potion == this->inventory_items[i])
        {
            index_to_remove = i;
        }
    }
    this->inventory_items.erase(this->inventory_items.begin() + index_to_remove);
    delete (potion);
}

void Inventory::remove_item(Item *item)
{
    int index_to_remove = 0;
    for (int i = 0; i < this->inventory_items.size(); i++)
    {
        if (item == this->inventory_items[i])
        {
            index_to_remove = i;
        }
    }
    this->inventory_items.erase(this->inventory_items.begin() + index_to_remove);
}