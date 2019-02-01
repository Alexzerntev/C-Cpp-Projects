#include "../../include/Livings/living.hpp"

Living::Living(int level, int healthPower, string name)
{
    this->level = level;
    this->healthPower = healthPower;
    this->max_healthPower = healthPower;
    this->name = name;
}

Living::~Living()
{
}

string Living::get_name()
{
    return this->name;
}

bool Living::is_stunned()
{
    return this->healthPower <= 0;
}

void Living::pass_effect()
{
    for (int i = 0; i < this->effects.size(); i++)
    {
        this->effects[i].reduce_duration();
    }
}

int Living::get_healthPower()
{
    int result = this->healthPower;
    for (int i = 0; i < this->effects.size(); i++)
    {
        if (this->effects[i].get_type() == HealthPower && !this->effects[i].is_passed())
        {
            result += this->effects[i].get_amount();
        }
    }
    return result;
}

void Living::set_healthPower(int hp)
{
    this->healthPower = hp;
}
