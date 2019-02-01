#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "include/user_interaction.hpp"
#include "include/worker_handler/worker_handler.hpp"

int main(int argc, char *argv[])
{
    UserInteraction *user_interaction = new UserInteraction();
    if (user_interaction == NULL)
    {
        printf("errno = % d \ n ", errno);
        perror("malloc");
    }
    if (!user_interaction->validate(argc, argv))
    {
        return error;
    }

    // for debug purpuses
    // WorkerHandler *worker_handler = new WorkerHandler(2, "./data/paths.txt");
    // if (worker_handler == NULL)
    // {
    //     printf("errno = % d \ n ", errno);
    //     perror("malloc");
    // }

    // final version
    WorkerHandler *worker_handler = new WorkerHandler(atoi(argv[4]), argv[2]);
    if (worker_handler == NULL)
    {
        printf("errno = % d \ n ", errno);
        perror("malloc");
    }
    worker_handler->initialize(user_interaction);

    delete (worker_handler);
    delete (user_interaction);

    return 0;
}