#ifndef MESSAGE_MANAGMENT_H
#define MESSAGE_MANAGMENT_H

// Includes
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>

// Constants
#define MAX_LINE_SIZE 100

// Function prototypes
int get_proccess_id_to_send(int uid);
int get_send_chance_value(int uid);

#endif