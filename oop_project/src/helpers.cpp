#include "../include/helpers.hpp"
#include "../include/enums.hpp"
#include "../include/data.hpp"
#include "../include/Livings/Heroes/hero.hpp"
#include "../include/Livings/Heroes/paladin.hpp"
#include "../include/Livings/Heroes/warrior.hpp"
#include "../include/Livings/Heroes/sorcerer.hpp"
#include "../include/Livings/Monsters/monster.hpp"

using namespace std;

int pure_text_menu(MenuType type)
{
    vector<string> messages;

    switch (type)
    {
    case FightMenu:
        messages = get_fight_menu();
        break;
    case TwoHandedMenu:
        messages = get_two_handed_menu();
        break;
    case InventoryMenu:
        messages = get_inventory_menu();
        break;
    case GlobalMenu:
        messages = get_global_menu();
        break;
    case MarketMenu:
        messages = get_market_menu();
        break;
    case MarketItemsMenu:
        messages = get_market_items_menu();
        break;
    case BinaryMenu:
        messages = get_binary_menu();
        break;
    }

    for (int i = 0; i < messages.size(); i++)
    {
        cout << messages[i] << endl;
    }

    bool validation = true;
    int action = 0;
    while (validation)
    {
        cin >> action;
        cout << endl;
        if (action < 1 || action > messages.size())
        {
            cout << "Wrong input, try again \n"
                 << endl;
            continue;
        }
        validation = false;
    }
    return action;
}

int number_validation(int size)
{
    bool validation = true;
    int action = 0;
    while (validation)
    {
        cin >> action;
        cout << endl;
        if (action < 1 || action > size)
        {
            cout << "Wrong input, try again \n"
                 << endl;
            continue;
        }
        validation = false;
    }
    return action;
}

Monster *monsters_menu(vector<Monster *> &monsters)
{
    for (int i = 0; i < monsters.size(); i++)
    {
        if (!monsters[i]->is_stunned())
        {
            cout << i + 1 << ". " << monsters[i]->get_name() << endl;
        }
    }

    bool validation = true;
    int action = 0;
    while (validation)
    {
        cin >> action;
        cout << endl;
        if (action < 1 || action > monsters.size() || monsters[action - 1]->is_stunned())
        {
            cout << "Wrong input, try again \n"
                 << endl;
            continue;
        }
        validation = false;
    }
    return monsters[action - 1];
}

Spell *spells_menu(vector<Spell *> &spells)
{
    for (int i = 0; i < spells.size(); i++)
    {
        cout << i + 1 << ". ";
        spells[i]->print();
        cout << endl;
    }

    cout << spells.size() + 1 << ". To return to previus menu" << endl;

    bool validation = true;
    int action = 0;
    while (validation)
    {
        cin >> action;
        cout << endl;
        if (action < 1 || action > spells.size() + 1)
        {
            cout << "Wrong input, try again \n"
                 << endl;
            continue;
        }
        validation = false;
    }
    if (action == spells.size() + 1)
    {
        return NULL;
    }
    return spells[action - 1];
}

Potion *potion_menu(vector<Potion *> &potions)
{
    for (int i = 0; i < potions.size(); i++)
    {
        cout << i + 1 << ". ";
        potions[i]->print();
        cout << endl;
    }

    cout << potions.size() + 1 << ". To return to previus menu" << endl;

    bool validation = true;
    int action = 0;
    while (validation)
    {
        cin >> action;
        cout << endl;
        if (action < 1 || action > potions.size() + 1)
        {
            cout << "Wrong input, try again \n"
                 << endl;
            continue;
        }
        validation = false;
    }
    if (action == potions.size() + 1)
    {
        return NULL;
    }
    return potions[action - 1];
}

Item *equipable_items_menu(vector<Item *> &items)
{
    for (int i = 0; i < items.size(); i++)
    {
        cout << i + 1 << ". ";
        items[i]->print();
        cout << endl;
    }
    if (items.size() == 0)
    {
        cout << "The inventory has no equipable items" << endl;
    }
    cout << items.size() + 1 << ". To return to previus menu" << endl;

    bool validation = true;
    int action = 0;
    while (validation)
    {
        cin >> action;
        cout << endl;
        if (action < 1 || action > items.size() + 1)
        {
            cout << "Wrong input, try again \n"
                 << endl;
            continue;
        }
        validation = false;
    }
    if (action == items.size() + 1)
    {
        return NULL;
    }
    return items[action - 1];
}

Hero *heroes_menu(vector<Hero *> &heroes)
{
    for (int i = 0; i < heroes.size(); i++)
    {
        cout << i + 1 << ". " << heroes[i]->get_name() << endl;
    }

    bool validation = true;
    int action = 0;
    while (validation)
    {
        cin >> action;
        cout << endl;
        if (action < 1 || action > heroes.size() || heroes[action - 1]->is_stunned())
        {
            cout << "Wrong input, try again \n"
                 << endl;
            continue;
        }
        validation = false;
    }
    return heroes[action - 1];
}