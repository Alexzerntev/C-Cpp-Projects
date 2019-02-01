#include "../../include/Grid/market_square.hpp"
#include "../../include/enums.hpp"
#include "../../include/Usables/Items/armor.hpp"
#include "../../include/Usables/Items/weapon.hpp"
#include "../../include/Usables/Items/potion.hpp"
#include "../../include/Usables/Spells/spell.hpp"

using namespace std;

MarketSquare::MarketSquare()
{
}
MarketSquare::MarketSquare(int market_index)
{
    switch (market_index)
    {
        {
        case 1:
            this->weapons.push_back(new Weapon("Sword of Bravery", 10, 2, 5, false));
            this->weapons.push_back(new Weapon("Staff of Wisdom", 14, 3, 9, false));
            this->weapons.push_back(new Weapon("Dance Macabre", 20, 3, 13, true));
            this->weapons.push_back(new Weapon("Mage Masher", 30, 5, 26, false));
            this->weapons.push_back(new Weapon("Hunter's Sword", 70, 8, 47, true));

            this->armors.push_back(new Armor("Emblem", 10, 2, 6));
            this->armors.push_back(new Armor("Ruby Shield", 17, 3, 16));
            this->armors.push_back(new Armor("Faerie Shield", 60, 7, 38));
            this->armors.push_back(new Armor("Magister Shield", 50, 6, 29));
            this->armors.push_back(new Armor("Moon Shield", 70, 8, 46));

            this->potions.push_back(new Potion("Soft", 10, 2, Agility, 10, 3));
            this->potions.push_back(new Potion("Elixir", 40, 5, HealthPower, 40, 3));
            this->potions.push_back(new Potion("Ether", 10, 2, Strength, 10, 3));
            this->potions.push_back(new Potion("Frag Grenade", 30, 4, Dexterity, 35, 3));
            this->potions.push_back(new Potion("Dream Power", 50, 7, MagicPower, 55, 3));

            this->spells.push_back(new FireSpell("Flame", 10, 2, 5, 8, 10, 3));
            this->spells.push_back(new FireSpell("Blaze", 40, 6, 5, 35, 40, 3));
            this->spells.push_back(new IceSpell("Chasm", 30, 3, 35, 20, 20, 3));
            this->spells.push_back(new LightningSpell("Fury of Osano-Wo", 60, 6, 40, 35, 40, 3));
            this->spells.push_back(new LightningSpell("Whispered Blade", 10, 2, 15, 8, 10, 3));

            break;
        case 2:
            this->weapons.push_back(new Weapon("Sidewinder", 11, 2, 7, false));
            this->weapons.push_back(new Weapon("Nightmare", 15, 3, 12, false));
            this->weapons.push_back(new Weapon("Knight Sword", 20, 4, 21, false));
            this->weapons.push_back(new Weapon("Twilight Steel", 40, 7, 46, false));
            this->weapons.push_back(new Weapon("Ice Brand", 70, 8, 50, true));

            this->armors.push_back(new Armor("Emblem", 10, 2, 6));
            this->armors.push_back(new Armor("Platinum Shield", 18, 2, 15));
            this->armors.push_back(new Armor("Sapphire Shield", 50, 6, 35));
            this->armors.push_back(new Armor("Serene Shield", 60, 8, 46));
            this->armors.push_back(new Armor("Soft Shield", 70, 9, 52));

            this->potions.push_back(new Potion("Hi-Potion", 10, 2, Agility, 10, 3));
            this->potions.push_back(new Potion("Turbo Ether", 10, 5, HealthPower, 30, 3));
            this->potions.push_back(new Potion("Poison Fang", 15, 2, Strength, 12, 3));
            this->potions.push_back(new Potion("Lunar Curtain", 23, 3, Dexterity, 20, 3));
            this->potions.push_back(new Potion("Mana Tabletr", 50, 8, MagicPower, 40, 3));

            this->spells.push_back(new FireSpell("Torch", 15, 2, 5, 18, 15, 3));
            this->spells.push_back(new FireSpell("Inferno", 60, 8, 5, 40, 50, 3));
            this->spells.push_back(new IceSpell("Nettle", 15, 2, 25, 18, 15, 3));
            this->spells.push_back(new LightningSpell("Yakamo’s Eye", 35, 3, 30, 15, 19, 3));
            this->spells.push_back(new LightningSpell("Whispered Blade", 10, 2, 15, 8, 10, 3));

            break;
        case 3:
            this->weapons.push_back(new Weapon("Nirvana", 10, 2, 7, true));
            this->weapons.push_back(new Weapon("Laevatein", 23, 4, 15, false));
            this->weapons.push_back(new Weapon("Tri-Rod", 25, 4, 18, false));
            this->weapons.push_back(new Weapon("Nemesis Rod", 30, 7, 36, false));
            this->weapons.push_back(new Weapon("Prism Rod", 100, 9, 58, false));

            this->armors.push_back(new Armor("White Shield", 10, 2, 6));
            this->armors.push_back(new Armor("Spiritual Shield", 15, 3, 15));
            this->armors.push_back(new Armor("Nimbus Rod", 20, 4, 21));
            this->armors.push_back(new Armor("Protect Shield", 40, 7, 38));
            this->armors.push_back(new Armor("Reflect Shield", 50, 8, 46));

            this->potions.push_back(new Potion("Stamina Spring", 10, 2, Agility, 10, 3));
            this->potions.push_back(new Potion("Healing Spring", 10, 2, HealthPower, 10, 3));
            this->potions.push_back(new Potion("Amulet", 10, 2, Strength, 10, 3));
            this->potions.push_back(new Potion("Frag Grenade", 20, 3, Dexterity, 15, 3));
            this->potions.push_back(new Potion("Wings to Discovery", 10, 4, MagicPower, 15, 3));

            this->spells.push_back(new FireSpell("Blaze", 40, 6, 5, 35, 40, 3));
            this->spells.push_back(new IceSpell("Explosion", 20, 2, 25, 15, 10, 3));
            this->spells.push_back(new IceSpell("Void", 40, 6, 45, 35, 40, 3));
            this->spells.push_back(new LightningSpell("Kharmic Intent", 30, 2, 25, 20, 20, 3));
            this->spells.push_back(new LightningSpell("Whispered Blade", 10, 2, 15, 8, 10, 3));

            break;
        default:

            break;
        }
    }
}
MarketSquare::~MarketSquare() {}

void MarketSquare::add_usable(Usable *usable)
{
    if (Weapon *wpn = dynamic_cast<Weapon *>(usable))
    {
        this->add_to_weapons(wpn);
    }
    else if (Potion *pot = dynamic_cast<Potion *>(usable))
    {
        this->add_to_potions(pot);
    }
    else if (Armor *armor = dynamic_cast<Armor *>(usable))
    {
        this->add_to_armors(armor);
    }
    else if (Spell *spell = dynamic_cast<Spell *>(usable))
    {
        this->add_to_spells(spell);
    }
}

void MarketSquare::add_to_weapons(Weapon *weapon)
{
    this->weapons.push_back(weapon);
}
void MarketSquare::add_to_armors(Armor *armor)
{
    this->armors.push_back(armor);
}
void MarketSquare::add_to_spells(Spell *spell)
{
    this->spells.push_back(spell);
}
void MarketSquare::add_to_potions(Potion *potion)
{
    this->potions.push_back(potion);
}

void MarketSquare::print_weapons()
{
    int i;
    for (i = 0; i < this->weapons.size(); i++)
    {
        cout << i + 1 << ") ";
        this->weapons[i]->print();
        cout << endl;
    }
}
void MarketSquare::print_armors()
{
    int i;
    for (i = 0; i < this->armors.size(); i++)
    {
        cout << i + 1 << ") ";
        this->armors[i]->print();
        cout << endl;
    }
}
void MarketSquare::print_spells()
{
    int i;
    for (i = 0; i < this->spells.size(); i++)
    {
        cout << i + 1 << ") ";
        this->spells[i]->print();
        cout << endl;
    }
}
void MarketSquare::print_potions()
{
    int i;
    for (i = 0; i < this->potions.size(); i++)
    {
        cout << i + 1 << ") ";
        this->potions[i]->print();
        cout << endl;
    }
}

int MarketSquare::get_collection_size(UsableType usableType)
{
    switch (usableType)
    {
    case WeaponType:
        return this->weapons.size();
    case ArmorType:
        return this->armors.size();
    case PotionType:
        return this->potions.size();
    case SpellType:
        return this->spells.size();
    }
}

Usable *MarketSquare::get_item_by_index(int index, UsableType usableType)
{
    switch (usableType)
    {
    case WeaponType:
        return this->weapons[index];
    case ArmorType:
        return this->armors[index];
    case PotionType:
        return this->potions[index];
    case SpellType:
        return this->spells[index];
    }
}

void MarketSquare::erase_item_by_index(int index, UsableType usableType)
{
    switch (usableType)
    {
    case WeaponType:
        this->weapons.erase(this->weapons.begin() + index);
        return;
    case ArmorType:
        this->armors.erase(this->armors.begin() + index);
        return;
    case PotionType:
        this->potions.erase(this->potions.begin() + index);
        return;
    case SpellType:
        this->spells.erase(this->spells.begin() + index);
        return;
    }
}
