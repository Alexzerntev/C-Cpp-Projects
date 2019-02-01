#include "../../include/Usables/effect.hpp"

Effect::Effect(int rounds, int amount, Ability type)
{
    this->amount = amount;
    this->rounds = rounds;
    this->type = type;
}

Effect::~Effect()
{
}

int Effect::get_rounds()
{
    return this->rounds;
}
Ability Effect::get_type()
{
    return this->type;
}
int Effect::get_amount()
{
    return this->amount;
}

void Effect::reduce_duration()
{
    this->rounds--;
}

bool Effect::is_passed()
{
    return this->rounds <= 0;
}
