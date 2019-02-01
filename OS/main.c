#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include "functions/intercom_managment.h"
#include "functions/shared_managment.h"
#include "data_structures/list.h"

int main(int argc, char *argv[])
{
    // Reading the arguments
    srand(time(NULL));
    int ct;
    if (argc != 2)
    {
        printf("Error, wrong argument count");
        return -1;
    }
    ct = atoi(argv[1]);
    if (ct == 0)
    {
        printf("Error, argument must be an Integer");
        return -1;
    }
    // Consts

    // Initialiazations
    int pids[3];

    for (int i = 0; i < 3; i++)
    {
        pids[i] = -1;
    }

    int id = 3;
    shared_memory *(data_sm[3][2]), *(checker_sm[3][2]);
    int checker_sem_ids[3], data_sem_ids[3];

    for (int i = 0; i < 3; i++)
    {
        checker_sem_ids[i] = sem_create(2); // 0 is Input 1 is Output
        for (int j = 0; j < 2; j++)
        {
            sem_init(checker_sem_ids[i], j, 1);
        }
        data_sem_ids[i] = sem_create(1); // Needed only for Output
        sem_init(data_sem_ids[i], 0, 1);
    }

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            data_sm[i][j] = get_shared_memory(sizeof(Container));
            checker_sm[i][j] = get_shared_memory(sizeof(Checker));
            Checker init;
            init.has_items = 0;
            init.ticks_executed = 0;
            memcpy(checker_sm[i][j]->shared_mem_ptr, &init, sizeof(Checker));
        }
    }

    for (int i = 0; i < 3; i++)
    {
        pids[i] = fork();

        if (pids[i] == 0) // if its child
        {
            child_operate(i,
                          ct,
                          checker_sem_ids[i],
                          data_sem_ids[i],
                          data_sm[i],
                          checker_sm[i]);
        }
    }

    // Parrent actions
    parent_operate(
        ct,
        data_sm,
        checker_sm,
        checker_sem_ids,
        data_sem_ids);

    for (int i = 0; i < 3; i++)
    {
        sem_dispose(checker_sem_ids[i], 0);
        sem_dispose(data_sem_ids[i], 0);
    }

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            dispose_shared_memory(data_sm[i][j]);
            dispose_shared_memory(checker_sm[i][j]);
        }
    }

    printf("The programm terminated successfully!");

    return 0;
}
