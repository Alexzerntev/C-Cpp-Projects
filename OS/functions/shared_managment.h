#ifndef SHARED_MANAGMENT_H
#define SHARED_MANAGMENT_H

// Includes
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <errno.h>

// Structs
typedef struct shared_memory
{
    int shm_id;
    void *shared_mem_ptr;
    int size;
} shared_memory;

// Function prototypes
int sem_create(int num_semaphores);
void sem_init(int semid, int index, int value);
void up(int semid, int index);
void down(int semid, int index);
void sem_dispose(int semid, int index);
shared_memory *get_shared_memory(int size);
void dispose_shared_memory(shared_memory *sm);

#endif