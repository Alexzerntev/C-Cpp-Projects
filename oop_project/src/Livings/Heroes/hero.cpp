#include "../../../include/Livings/Heroes/hero.hpp"
#include "../../../include/helpers.hpp" // do not remove to .hpp cos of cyclic dependency
#include "../../../include/Livings/Monsters/monster.hpp"
#include "../../../include/Livings/Heroes/paladin.hpp"
#include "../../../include/Livings/Heroes/sorcerer.hpp"
#include "../../../include/Livings/Heroes/warrior.hpp"

Hero::Hero(int level, int healthPower, string name, int magicPower,
           int strength, int dexterity, int agility, int money, int experience, HeroType heroType)
    : Living(level, healthPower, name)
{
    this->magicPower = magicPower;
    this->max_MagicPower = magicPower;
    this->strength = strength;
    this->dexterity = dexterity;
    this->agility = agility;
    this->money = money;
    this->experience = experience;
    this->experience_to_next_level = 50;
    this->invertory = new Inventory(heroType);

    switch (heroType)
    {
    case SorcererType:
        this->weapon[0] = new Weapon("Staff of the first born", 10, 1, 5, false);
        this->weapon[1] = NULL;
        this->armor = new Armor("Cloth", 10, 1, 2);
        break;
    case PaladinType:
        this->weapon[0] = new Weapon("Hammer of the first born", 10, 1, 5, false);
        this->weapon[1] = NULL;
        this->armor = new Armor("Cloth", 10, 1, 2);
        break;
    case WarriorType:
        this->weapon[0] = new Weapon("Sword of the first born", 10, 1, 5, false);
        this->weapon[1] = NULL;
        this->armor = new Armor("Cloth", 10, 1, 2);
        break;
    }
}

Hero::~Hero()
{
}

void Hero::print_inventory()
{
    this->invertory->print();
}

int Hero::get_inventory_size()
{
    return this->invertory->get_size();
}

void Hero::attack(Monster *monster)
{
    monster->take_attack(this->get_damage()); // not just getter, but complex function
    if (monster->is_stunned())
    {
        cout << this->get_name() << " Killed " << monster->get_name();
    }
}

void Hero::take_attack(int damage)
{
    int chance_to_dodge = rand() % 100;
    if (this->agility * 0.5 >= chance_to_dodge)
    {
        this->get_name();
        cout << this->get_name() << " has dodged the attack" << endl;
        return;
    }
    int deffence = 0;
    if (this->armor != NULL)
    {
        deffence = this->armor->get_defence() * (0.3);
    }
    cout << this->get_name() << " had been attacked for " << deffence - damage << " damage" << endl;
    this->set_healthPower(this->get_healthPower() + deffence - damage);
}

Usable *Hero::sell_item(int index)
{
    this->money = this->money + this->invertory->get_item(index)->get_price();
    return this->invertory->remove_from_inventory(index);
}

int Hero::buy_item(Usable *usable)
{
    if (usable->get_price() > this->money)
    {
        cout << "Not enough money" << endl;
        return 0;
    }
    this->money = this->money - usable->get_price();
    this->invertory->add_item(usable);
    return 1;
}

int Hero::get_damage()
{
    int result = 0;
    for (int i = 0; i < 2; i++)
    {
        if (this->weapon[i] != NULL)
        {
            result += this->weapon[i]->get_damage();
        }
    }
    result += this->strength * (0.3);
    return result;
}

void Hero::print_invertory_wepons()
{
    this->invertory->print_weapon();
}

void Hero::print_inverory_armors()
{
    this->invertory->print_armor();
}

Spell *Hero::choose_spell_to_cast()
{
    bool validation = false;
    Spell *spell;
    while (!validation)
    {
        vector<Spell *> spells = this->invertory->get_spells();
        spell = spells_menu(spells);
        if (spell == NULL)
        {
            return NULL;
        }
        if (spell->get_minimum_level() > this->level)
        {
            cout << "Too low level to use" << endl;
            continue;
        }
        if (spell->get_needed_magic_power() > this->get_magicPower())
        {
            cout << "Not Enough magic power" << endl;
            continue;
        }
        validation = true;
    }
    return spell;
}

void Hero::cast_spell(Monster *monster, Spell *spell)
{
    if (FireSpell *fs = dynamic_cast<FireSpell *>(spell))
    {
        monster->take_spell_attack(DefenceReduction, fs->get_defence_reduction(), fs->get_duration(),
                                   fs->get_damage() + (0.3 * this->dexterity));
    }
    if (IceSpell *is = dynamic_cast<IceSpell *>(spell))
    {
        monster->take_spell_attack(DamageReduction, is->get_damage_reduction(), is->get_duration(),
                                   is->get_damage() + (0.3 * this->dexterity));
    }
    if (LightningSpell *ls = dynamic_cast<LightningSpell *>(spell))
    {
        monster->take_spell_attack(DodgeReduction, ls->get_dodge_reduction(), ls->get_duration(),
                                   ls->get_damage() + (0.3 * this->dexterity));
    }
}

Potion *Hero::choose_potion_to_drink()
{
    bool validation = false;
    Potion *potion;
    while (!validation)
    {
        vector<Potion *> potions = this->invertory->get_potions();
        potion = potion_menu(potions);
        if (potion == NULL)
        {
            return NULL;
        }
        if (potion->get_minimum_level() > this->level)
        {
            cout << "Too low level to use" << endl;
            continue;
        }
        validation = true;
    }
    return potion;
}

void Hero::drink_potion(Potion *potion)
{
    Effect effect(potion->get_duration(), potion->get_amount(), potion->get_type());
    this->effects.push_back(effect);
    this->invertory->remove_potion(potion);
}

Item *Hero::choose_item_to_equip()
{
    bool validation = false;
    Item *item;
    while (!validation)
    {
        vector<Item *> items = this->invertory->get_equipable_items();
        item = equipable_items_menu(items);
        if (item == NULL)
        {
            return NULL;
        }
        if (item->get_minimum_level() > this->level)
        {
            cout << "Too low level to use" << endl;
            continue;
        }
        validation = true;
    }
    return item;
}

void Hero::equip_item(Item *item)
{
    this->invertory->remove_item(item);
    if (Weapon *wpn = dynamic_cast<Weapon *>(item))
    {
        if (wpn->get_is_two_handed())
        {
            if (this->weapon[0] != NULL)
            {
                this->invertory->add_item(this->weapon[0]);
            }
            if (this->weapon[1] != NULL)
            {
                this->invertory->add_item(this->weapon[1]);
            }
            this->weapon[0] = wpn;
            this->weapon[1] = NULL;
        }
        else
        {
            int action = pure_text_menu(TwoHandedMenu);
            this->invertory->add_item(this->weapon[action - 1]);
            this->weapon[action - 1] = wpn;
        }
    }
    if (Armor *armr = dynamic_cast<Armor *>(item))
    {
        if (this->armor != NULL)
        {
            this->invertory->add_item(this->armor);
        }
        this->armor = armr;
    }

    cout << "The item has been equiped" << endl;
}

void Hero::after_fight_win(int monsters_killed)
{
    if (this->is_stunned())
    {
        this->set_healthPower(this->max_healthPower / 2);
    }
    this->money = this->money + (monsters_killed * this->level * 10);
    this->experience = this->experience + (monsters_killed * this->level * 5);
    this->magicPower = this->max_MagicPower;
    this->effects.clear();
    if (this->experience > this->experience_to_next_level)
    {
        this->level_up();
    }
}

void Hero::after_fight_lose()
{
    if (this->is_stunned())
    {
        this->set_healthPower(this->max_healthPower / 2);
        this->money = this->money / 2;
    }
    this->effects.clear();
    this->magicPower = this->max_MagicPower;
}

void Hero::level_up()
{
    this->level++;
    this->experience_to_next_level = this->experience_to_next_level * 2;
    this->max_healthPower = this->max_healthPower * 1.2;
    this->set_healthPower(this->max_healthPower);

    if (Paladin *pal = dynamic_cast<Paladin *>(this))
    {
        this->dexterity += this->level * 3;
        this->agility += this->level * 2;
        this->strength += this->level * 3;
    }
    if (Warrior *war = dynamic_cast<Warrior *>(this))
    {
        this->dexterity += this->level * 2;
        this->agility += this->level * 3;
        this->strength += this->level * 3;
    }
    if (Sorcerer *pal = dynamic_cast<Sorcerer *>(this))
    {
        this->dexterity += this->level * 3;
        this->agility += this->level * 3;
        this->strength += this->level * 2;
    }
}

int Hero::get_magicPower()
{
    int result = this->magicPower;
    for (int i = 0; i < this->effects.size(); i++)
    {
        if (this->effects[i].get_type() == MagicPower)
        {
            result += this->effects[i].get_amount();
        }
    }
    return result;
}
int Hero::get_strength()
{
    int result = this->strength;
    for (int i = 0; i < this->effects.size(); i++)
    {
        if (this->effects[i].get_type() == Strength && !this->effects[i].is_passed())
        {
            result += this->effects[i].get_amount();
        }
    }
    return result;
}
int Hero::get_dexterity()
{
    int result = this->dexterity;
    for (int i = 0; i < this->effects.size(); i++)
    {
        if (this->effects[i].get_type() == Dexterity && !this->effects[i].is_passed())
        {
            result += this->effects[i].get_amount();
        }
    }
    return result;
}
int Hero::get_agility()
{
    int result = this->agility;
    for (int i = 0; i < this->effects.size(); i++)
    {
        if (this->effects[i].get_type() == Agility && !this->effects[i].is_passed())
        {
            result += this->effects[i].get_amount();
        }
    }
    return result;
}