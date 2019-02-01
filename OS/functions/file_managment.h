#ifndef FILE_MANAGMENT_H
#define FILE_MANAGMENT_H

// Includes
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Constants
#define MESSAGES_FILE_NAME "files/messages"
#define LOG_FILE_NAME "files/log/log_p_ "
#define MAX_MESSAGE_SIZE 100

// Function prototypes
char *get_random_message();
int get_line_count(FILE *fp);
void write_to_log(char *data, int uid);

#endif