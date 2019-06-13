#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#define SUCCESS 0
#define MEMORY_ALLOCATION_ERROR 1
#define WRONG_INITIALIZATION_ERROR 2
#define FLOAT_CONVERTION_ERROR 3
#define INT_CONVERTION_ERROR 4
#define READ_FILE_ERROR 5
#define STRING_PARSING_ERROR 6
#define DUPLICATE_USER_ERROR 7
#define USER_NOT_FOUND_ERROR 8
#define NOT_ENOUGH_MONEY_ERROR 9
#define BITCOIN_NOT_FOUND_ERROR 10
#define WRONG_INPUT 11

#include "errno.h"
#include <stdio.h>

void print_error(int error_code, const char *message);

#endif