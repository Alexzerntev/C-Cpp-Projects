#ifndef INTERCOM_MANAGMENT_H
#define INTERCOM_MANAGMENT_H

// Includes
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include "message_managment.h"
#include "file_managment.h"
#include "shared_managment.h"
#include "../data_structures/list.h"

// Constants
#define INPUT 0
#define OUTPUT 1

#define MAX_LOG_SIZE 200

typedef struct Checker
{
    int has_items;
    int ticks_executed;
} Checker;

void parent_operate(
    int ct,
    shared_memory *data_sm[3][2],
    shared_memory *checker_sm[3][2],
    int checker_sem_ids[3],
    int data_sem_ids[3]);

void child_operate(
    int uid,
    int ct_to_execute,
    int checker_sem_ids,
    int data_sem_id,
    shared_memory *data_sm[2],
    shared_memory *checker_sm[2]);
#endif