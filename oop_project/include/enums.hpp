#ifndef ENUMS_H
#define ENUMS_H
#include <iostream>
#include <string>

using namespace std;

enum Ability
{
    HealthPower,
    MagicPower,
    Strength,
    Dexterity,
    Agility,
    DodgeReduction,
    DefenceReduction,
    DamageReduction
};

enum HeroType
{
    PaladinType,
    SorcererType,
    WarriorType
};

enum UsableType
{
    WeaponType,
    ArmorType,
    PotionType,
    SpellType
};

enum MenuType
{
    FightMenu,
    TwoHandedMenu,
    InventoryMenu,
    GlobalMenu,
    MarketMenu,
    MarketItemsMenu,
    BinaryMenu
};

#endif