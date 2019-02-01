#include "./recomendation_helpers.hpp"

int get_coin_index(vector<string> *coins, string name)
{
    for (unsigned int i = 0; i < coins->size(); i++)
    {
        if (coins->at(i) == name)
        {
            return i;
        }
    }
    return -1;
}