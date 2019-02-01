#include "shared_managment.h"

// Semaphores managment
int sem_create(int num_semaphores)
{ /* procedure to create specified number of semaphores */
    int semid;

    /* create new semaphore set of semaphores */
    if ((semid = semget(IPC_PRIVATE, num_semaphores, IPC_CREAT | 0600)) < 0)
    {
        perror("error in creating semaphore"); /* 0600 = read/alter by user */
        exit(1);
    }
    return semid;
}

void sem_init(int semid, int index, int value)
{ /* procedure to initialize specified semaphore to given value */
    if (semctl(semid, index, SETVAL, value) < 0)
    {
        perror("error in initializing first semaphore");
        exit(1);
    }
}

void sem_dispose(int semid, int index)
{
    if (semctl(semid, index, IPC_RMID) < 0)
    {
        perror("error in removing semaphore from the system");
        exit(1);
    }
}

void down(int semid, int index)
{                          /* procedure to perform a P or wait operation on a semaphore of given index */
    struct sembuf sops[1]; /* only one semaphore operation to be executed */

    sops[0].sem_num = index; /* define operation on semaphore with given index */
    sops[0].sem_op = -1;     /* subtract 1 to value for P operation */
    sops[0].sem_flg = 0;     /* type "man semop" in shell window for details */

    if (semop(semid, sops, 1) == -1)
    {
        perror("error in semaphore operation");
        exit(1);
    }
}

void up(int semid, int index)
{                          /* procedure to perform a V or signal operation on semaphore of given index */
    struct sembuf sops[1]; /* define operation on semaphore with given index */

    sops[0].sem_num = index; /* define operation on semaphore with given index */
    sops[0].sem_op = 1;      /* add 1 to value for V operation */
    sops[0].sem_flg = 0;     /* type "man semop" in shell window for details */

    if (semop(semid, sops, 1) == -1)
    {
        perror("error in semaphore operation");
        exit(1);
    }
}

// Shared memory managment

shared_memory *get_shared_memory(int size)
{
    shared_memory *sm = (shared_memory *)malloc(sizeof(shared_memory));
    sm->size = size;
    if ((sm->shm_id = shmget(IPC_PRIVATE, sm->size, IPC_CREAT | 0600)) == -1)
    {
        perror("shmget()");
        exit(1);
    }
    if ((sm->shared_mem_ptr = (int *)shmat(sm->shm_id, 0, 0)) == (int *)-1)
    {
        perror("shmat()");
        exit(1);
    }
    return sm;
}

void dispose_shared_memory(shared_memory *sm)
{
    shmdt(sm->shared_mem_ptr);
    shmctl(sm->shm_id, IPC_RMID, 0);
    free(sm);
}