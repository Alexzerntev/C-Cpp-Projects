#include "../../../include/Livings/Monsters/monster.hpp"
#include "../../../include/Livings/Heroes/hero.hpp"

Monster::Monster(int level, int healthPower, string name, int damage, int defence, int flexibility) : Living(level, healthPower, name)
{
    this->damage = damage;
    this->defence = defence;
    this->flexibility = flexibility;
}
Monster::~Monster() {}

void Monster::take_attack(int damage)
{
    int chance_to_dodge = rand() % 100;
    if (this->flexibility * 0.5 >= chance_to_dodge)
    {
        this->get_name();
        cout << this->get_name() << " has dodged the attack" << endl;
        return;
    }
    cout << this->get_name() << " had been attacked for " << this->get_defence() * (0.3) - damage << " damage" << endl;
    this->set_healthPower(this->get_healthPower() + this->get_defence() * (0.3) - damage);
}

void Monster::take_spell_attack(Ability type, int amount, int duration, int damage)
{
    Effect effect(duration, amount, type);
    this->effects.push_back(effect);
    this->set_healthPower(this->get_healthPower() + this->get_defence() * (0.3) - damage);
}

void Monster::attack(Hero *hero)
{
    hero->take_attack(this->damage);
    if (hero->is_stunned())
    {
        cout << this->get_name() << " Killed " << hero->get_name() << endl;
    }
}

int Monster::get_damage()
{
    int result = this->damage;
    for (int i = 0; i < this->effects.size(); i++)
    {
        if (this->effects[i].get_type() == DamageReduction)
        {
            result -= this->effects[i].get_amount();
        }
    }
    if (result < 0)
    {
        result = 0;
    }
    return result;
}

int Monster::get_defence()
{
    int result = this->defence;
    for (int i = 0; i < this->effects.size(); i++)
    {
        if (this->effects[i].get_type() == DefenceReduction)
        {
            result -= this->effects[i].get_amount();
        }
    }
    if (result < 0)
    {
        result = 0;
    }
    return result;
}

int Monster::get_flexibility()
{
    int result = this->flexibility;
    for (int i = 0; i < this->effects.size(); i++)
    {
        if (this->effects[i].get_type() == DodgeReduction)
        {
            result -= this->effects[i].get_amount();
        }
    }
    if (result < 0)
    {
        result = 0;
    }
    return result;
}