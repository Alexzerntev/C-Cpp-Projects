#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "include/user_interaction.hpp"
#include "include/data_structures/map/map.hpp"

int main(int argc, char *argv[])
{
    UserInteraction *user_interaction = new UserInteraction();
    if (!user_interaction->validate(argc, argv))
    {
        return error;
    }

    // for debug purpuses
    //Map *map = new Map("Entertainment.txt");

    Map *map = new Map(argv[2]);

    if (!map->execute_mapping())
    {
        return error;
    }
    user_interaction->set_map(map);

    // for debug purpuses
    //map->print_strings();

    user_interaction->interact();

    delete (user_interaction);
    delete (map);

    return 0;
}

// ./minisearch -i docfile.txt -k 5