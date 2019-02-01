#include "../include/gameplay.hpp"

using namespace std;

Gameplay::Gameplay()
{
    this->x = 10;
    this->y = 1;
    cout << "Welcome to the game" << endl;
    int N = 0, input, i, j;
    bool flag = true;
    while (flag)
    {
        cout << "Please give me a number of Heroes" << endl;
        cin >> N;
        if (N <= 0 || N > 3)
        {
            cout << "Wrong input, try again \n"
                 << endl;
            continue;
        }
        flag = false;
    }
    Warrior *warrior = new Warrior(1, 100, "Abathon", 10, 20, 10, 20, 15, 0, WarriorType); // HeroType only for
    Paladin *paladin = new Paladin(1, 100, "Ehlonna", 10, 20, 20, 10, 15, 0, PaladinType); // Items initialization
    Sorcerer *sorcerer = new Sorcerer(1, 100, "Gadhelyn", 30, 10, 20, 20, 15, 0, SorcererType);

    bool chosen_heroes[3];
    chosen_heroes[0] = false;
    chosen_heroes[1] = false;
    chosen_heroes[2] = false;
    string heroes_to_print[3];
    heroes_to_print[0] = "To choose Abathon press 1";
    heroes_to_print[1] = "To choose Ehlonna press 2";
    heroes_to_print[2] = "To choose Gadhelyb press 3";

    cout << "The heroes to chose are : " << endl
         << endl;
    cout << "1) ";
    warrior->print();
    cout << endl
         << "2) ";
    paladin->print();
    cout << endl
         << "3) ";
    sorcerer->print();
    cout << endl;

    if (N == 3)
    {
        this->heroes.push_back(warrior);
        this->heroes.push_back(paladin);
        this->heroes.push_back(sorcerer);
        cout << "You have chosen all 3 heroes " << endl;
        return;
    }

    for (i = 0; i < N; i++)
    {
        flag = true;
        cout << "Please select your " << i + 1 << " hero" << endl;
        for (j = 0; j < 3; j++)
        {
            if (chosen_heroes[j] != true)
            {
                cout << heroes_to_print[j] << endl;
            }
        }
        while (flag)
        {
            cin >> input;
            if (input <= 0 || input > 3 || chosen_heroes[input - 1])
            {
                cout << "Wrong input, try again \n"
                     << endl;
                continue;
            }
            flag = false;
            switch (input)
            {
                {
                case 1:
                    this->heroes.push_back(warrior);
                    chosen_heroes[0] = true;
                    break;
                case 2:
                    this->heroes.push_back(paladin);
                    chosen_heroes[1] = true;
                    break;
                case 3:
                    this->heroes.push_back(sorcerer);
                    chosen_heroes[2] = true;
                    break;
                }
            }
        }
    }
}

int Gameplay::GetHeroesX()
{
    return this->x;
}

int Gameplay::GetHeroesY()
{
    return this->y;
}

int Gameplay::ShowOptions()
{
    bool flag = true;
    int choice = 0;
    cout << "Choose one of the options below: " << endl
         << endl;
    choice = pure_text_menu(GlobalMenu);
    switch (choice)
    {
    case 1:
        this->CheckInventory();
        break;
    case 2:
        this->gamplayGrid.DisplayMap(this->GetHeroesX(), this->GetHeroesY());
        break;
    case 3:
        this->Move();
        break;
    case 4:
        this->ShowStatistics();
        break;
    case 5:
        return 0;
    }
    return 1;
}

void Gameplay::CheckInventory()
{
    Hero *hero;
    Item *item_to_equip;
    int action = pure_text_menu(InventoryMenu);

    cout << "For which hero dou you want to Check the inventory?" << endl;
    hero = heroes_menu(this->heroes);
    switch (action)
    {
    case 1:
        hero->print_inventory();
        break;
    case 2:
        cout << "Which Item do you want to equip" << endl;
        item_to_equip = hero->choose_item_to_equip();
        if (item_to_equip == NULL)
        {
            return;
        }
        hero->equip_item(item_to_equip);
        break;
    }
}

void Gameplay::Move()
{
    int i;
    bool toPrint[5];
    toPrint[0] = true;
    toPrint[1] = true;
    toPrint[2] = true;
    toPrint[3] = true;
    toPrint[4] = true;
    string messages[5];
    messages[0] = "To Move Up Press 1";
    messages[1] = "To Move Down Press 2";
    messages[2] = "To Move Left Press 3";
    messages[3] = "To Move Right Press 4";
    messages[4] = "To Cancel Press 5";

    if (NonAccesableSquare *naSq3 = dynamic_cast<NonAccesableSquare *>(this->gamplayGrid.grid[this->x - 1][this->y]))
    {
        toPrint[0] = false;
    }
    if (NonAccesableSquare *naSq4 = dynamic_cast<NonAccesableSquare *>(this->gamplayGrid.grid[this->x + 1][this->y]))
    {
        toPrint[1] = false;
    }
    if (NonAccesableSquare *naSq2 = dynamic_cast<NonAccesableSquare *>(this->gamplayGrid.grid[this->x][this->y - 1]))
    {
        toPrint[2] = false;
    }
    if (NonAccesableSquare *naSq1 = dynamic_cast<NonAccesableSquare *>(this->gamplayGrid.grid[this->x][this->y + 1]))
    {
        toPrint[3] = false;
    }

    for (i = 0; i < 5; i++)
    {
        if (toPrint[i] == true)
        {
            cout << messages[i] << endl;
        }
    }

    bool flag = true;
    int choice = 0;
    while (flag)
    {
        cin >> choice;
        if (choice < 1 || choice > 5 || !toPrint[choice - 1])
        {
            cout << "Wrong input, try again \n"
                 << endl;
            continue;
        }
        flag = false;
    }
    switch (choice)
    {
    case 1:
        this->x = this->x - 1;
        break;
    case 2:
        this->x = this->x + 1;
        break;
    case 3:
        this->y = this->y - 1;
        break;
    case 4:
        this->y = this->y + 1;
        break;
    case 5:
        return;
    }

    if (MarketSquare *maSq = dynamic_cast<MarketSquare *>(this->gamplayGrid.grid[this->x][this->y]))
    {
        this->GoToTheMarket(maSq);
    }

    if (CommonSquare *coSq = dynamic_cast<CommonSquare *>(this->gamplayGrid.grid[this->x][this->y]))
    {
        int chance = rand() % 100;
        if (chance < 30)
        {
            int monsters = rand() % 5;
            for (int i = 0; i < monsters; i++)
            {
                coSq->monsters_on_squere.push_back(this->get_random_Monster(this->heroes[0]));
            }
        }
        if (coSq->monsters_on_squere.size() != 0)
        {
            this->Fight(coSq);
            coSq->monsters_on_squere.clear();
        }
    }
}

void Gameplay::ShowStatistics()
{
    for (int i = 0; i < this->heroes.size(); i++)
    {
        this->heroes[i]->print();
        cout << endl;
    }
}

void Gameplay::Fight(CommonSquare *coSq)
{
    cout << "You are fighting: " << endl;
    coSq->print_monsters();

    int action = 0;
    Monster *monster_to_attack;
    Spell *spell_to_cast;
    Potion *potion_to_drink;
    Item *item_to_equip;

    while (true)
    {
        for (int i = 0; i < this->heroes.size(); i++)
        {
            if (!this->heroes[i]->is_stunned())
            {
                cout << "It's " << this->heroes[i]->get_name() << " turn, what do you want to do?" << endl
                     << endl;
                action = pure_text_menu(FightMenu);
                switch (action)
                {
                case 1:
                    cout << "Which monster do you want to attack?" << endl;
                    monster_to_attack = monsters_menu(coSq->monsters_on_squere);
                    this->heroes[i]->attack(monster_to_attack);
                    break;
                case 2:
                    cout << "Which monster do you want to cast a spell on?" << endl;
                    monster_to_attack = monsters_menu(coSq->monsters_on_squere);
                    spell_to_cast = this->heroes[i]->choose_spell_to_cast();
                    if (spell_to_cast == NULL)
                    {
                        i--;
                        break;
                    }
                    this->heroes[i]->cast_spell(monster_to_attack, spell_to_cast);
                    break;
                case 3:
                    cout << "Which potion do you want to drink" << endl;
                    potion_to_drink = this->heroes[i]->choose_potion_to_drink();
                    if (potion_to_drink == NULL)
                    {
                        i--;
                        break;
                    }
                    this->heroes[i]->drink_potion(potion_to_drink);
                    break;
                case 4:
                    cout << "Which Item do you want to equip" << endl;
                    item_to_equip = this->heroes[i]->choose_item_to_equip();
                    if (item_to_equip == NULL)
                    {
                        i--;
                        break;
                    }
                    this->heroes[i]->equip_item(item_to_equip);
                    break;
                case 5:
                    this->heroes[i]->print();
                    cout << endl;
                    for (int j = 0; j < coSq->monsters_on_squere.size(); j++)
                    {
                        coSq->monsters_on_squere[j]->print();
                        cout << endl;
                    }
                    i--;
                    break;
                }
            }
        }

        for (int i = 0; i < coSq->monsters_on_squere.size(); i++)
        {
            int hero_to_attack = -1;
            for (int j = 0; j < this->heroes.size(); j++)
            {
                if (!this->heroes[j]->is_stunned())
                {
                    hero_to_attack = j;
                    break;
                }
            }
            if (hero_to_attack == -1)
            {
                break;
            }
            if (!coSq->monsters_on_squere[i]->is_stunned())
            {
                coSq->monsters_on_squere[i]->attack(this->heroes[hero_to_attack]);
            }
        }

        int count = 0;
        for (int i = 0; i < coSq->monsters_on_squere.size(); i++)
        {
            if (!coSq->monsters_on_squere[i]->is_stunned())
            {
                count++;
            }
            coSq->monsters_on_squere[i]->pass_effect();
        }
        if (count == 0) // case all monster died
        {
            for (int i = 0; i < this->heroes.size(); i++)
            {
                this->heroes[i]->after_fight_win(coSq->monsters_on_squere.size());
            }
            cout << "Monsters lost the battle" << endl;
            return;
        }

        count = 0;
        for (int i = 0; i < this->heroes.size(); i++)
        {
            if (!this->heroes[i]->is_stunned())
            {
                count++;
            }
            this->heroes[i]->pass_effect();
        }
        if (count == 0) // case all heroes died
        {
            for (int i = 0; i < this->heroes.size(); i++)
            {
                this->heroes[i]->after_fight_lose();
            }
            cout << "Heroes lost the battle" << endl;
            return;
        }
    }
}

void Gameplay::GoToTheMarket(MarketSquare *maSq)
{
    // Global section starts =============================================
    int i = 0;
    while (true)
    {
        cout << "You are on the Market" << endl
             << endl;

        bool menuFlag = true;
        int menuChoise = pure_text_menu(MarketMenu);
        if (menuChoise == 3)
        {
            return;
        }
        cout << "For Which hero do you want to Sell/Buy?" << endl;
        Hero *hero;
        hero = heroes_menu(this->heroes);
        // Global section ends =============================================================
        if (menuChoise == 1) // Selling case starts ----------------------------------------
        {
            if (hero->get_inventory_size() == 0)
            {
                cout << "The inventory is empty" << endl;
            }
            else
            {
                hero->print_inventory();

                cout << "Which one do you want to sell?" << endl;
                int itemIndex = number_validation(hero->get_inventory_size());

                Usable *soldItem = hero->sell_item(itemIndex - 1);
                maSq->add_usable(soldItem);
            }
        }
        // Selling case ends -------------------------------------------------------

        if (menuChoise == 2) // Buying case starts --------------------------------------------------------
        {
            UsableType usable_type;

            cout << "What do you want to buy?" << endl
                 << endl;
            int typeChoise = pure_text_menu(MarketItemsMenu);
            switch (typeChoise)
            {
            case 1:
                maSq->print_weapons();
                usable_type = WeaponType;
                break;
            case 2:
                maSq->print_armors();
                usable_type = ArmorType;
                break;
            case 3:
                maSq->print_spells();
                usable_type = SpellType;
                break;
            case 4:
                maSq->print_potions();
                usable_type = PotionType;
                break;
            case 5:
                return;
            }

            cout << "Which one do you want to Buy?" << endl;
            int itemIndexChoise = 0;
            bool itemIndexFlag = true;
            while (itemIndexFlag)
            {
                cin >> itemIndexChoise;
                cout << endl;
                if (itemIndexChoise < 1 || itemIndexChoise > maSq->get_collection_size(usable_type))
                {
                    cout << "Wrong input, try again \n"
                         << endl;
                    continue;
                }
                itemIndexFlag = false;
            }
            int result = hero->buy_item(maSq->get_item_by_index(itemIndexChoise - 1, usable_type));
            if (result != 0) // in case of succses
            {
                maSq->erase_item_by_index(itemIndexChoise - 1, usable_type);
            }
        }
        // Buying case ends ------------------------------------------------------------

        // Global section starts ======================================================
        cout << "Do you want to make another Buy/Sell ?" << endl;
        int finalChoice = pure_text_menu(BinaryMenu);
        if (finalChoice == 2)
        {
            return;
        }
        // Global section ends  =============================================
    }
}

Monster *Gameplay::get_random_Monster(Hero *hero)
{
    int result = rand() % 3;
    Monster *monster;
    Dragon *dragon;
    Exoskeleton *exoskeleton;
    Spirit *spirit;
    switch (result)
    {
    case 0:
        dragon = new Dragon(hero->get_level(), hero->get_healthPower(),
                            get_random_monster_name(), hero->get_level() * 12,
                            hero->get_level() * 10, hero->get_level() * 10);
        monster = dragon;
        break;
    case 1:
        exoskeleton = new Exoskeleton(hero->get_level(), hero->get_healthPower(),
                                      get_random_monster_name(), hero->get_level() * 10,
                                      hero->get_level() * 12, hero->get_level() * 10);
        monster = exoskeleton;
        break;
    case 2:
        spirit = new Spirit(hero->get_level(), hero->get_healthPower(),
                            get_random_monster_name(), hero->get_level() * 10,
                            hero->get_level() * 10, hero->get_level() * 12);
        monster = spirit;
        break;
    }
    return monster;
}

Gameplay::~Gameplay() {}