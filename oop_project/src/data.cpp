#include "../include/helpers.hpp"

using namespace std;

vector<string> get_fight_menu()
{
    vector<string> messages;
    messages.push_back("1. Attack");
    messages.push_back("2. Cast Spell");
    messages.push_back("3. Use Potion");
    messages.push_back("4. Equip Item");
    messages.push_back("5. Display Stats");
    return messages;
}

vector<string> get_two_handed_menu()
{
    vector<string> messages;
    messages.push_back("1. Left Hand");
    messages.push_back("2. Right Hand");
    return messages;
}

vector<string> get_inventory_menu()
{
    vector<string> messages;
    messages.push_back("1. Show Inventory");
    messages.push_back("2. Equip Item");
    return messages;
}

vector<string> get_global_menu()
{
    vector<string> messages;
    messages.push_back("1. Check Inventory");
    messages.push_back("2. Display Map");
    messages.push_back("3. Move");
    messages.push_back("4. Show Heroes Statistics");
    messages.push_back("5. Quit Game");
    return messages;
}

vector<string> get_market_menu()
{
    vector<string> messages;
    messages.push_back("1. Selling items");
    messages.push_back("2. Buying items");
    messages.push_back("3. Leave");
    return messages;
}

vector<string> get_market_items_menu()
{
    vector<string> messages;
    messages.push_back("1. Weapons");
    messages.push_back("2. Armors");
    messages.push_back("3. Spells");
    messages.push_back("4. Potions");
    messages.push_back("5. Leave Market");
    return messages;
}

vector<string> get_binary_menu()
{
    vector<string> messages;
    messages.push_back("1. Yes");
    messages.push_back("2. No");
    return messages;
}

string get_random_monster_name()
{
    int result = rand() % 20;
    switch (result)
    {
    case 0:
        return "Abbathor";
    case 1:
        return "Arvoreen";
    case 2:
        return "Cyrrollalee";
    case 3:
        return "Charmalaine";
    case 4:
        return "Brandobaris";
    case 5:
        return "Dumathoin";
    case 6:
        return "Ehlonna";
    case 7:
        return "Fharlanghn";
    case 8:
        return "Moradin";
    case 9:
        return "Olidammara";
    case 10:
        return "Pelor";
    case 11:
        return "Hruggek";
    case 12:
        return "Yeenoghu";
    case 13:
        return "Falazure";
    case 14:
        return "Hiatea";
    case 15:
        return "Karontor";
    case 16:
        return "Kiaransalee";
    case 17:
        return "Khurgorbaeyag";
    case 18:
        return "Luthic";
    case 19:
        return "Memnor";
    }
}