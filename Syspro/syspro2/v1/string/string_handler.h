#ifndef STRING_HANDLER_H
#define STRING_HANDLER_H

#include <stdlib.h>
#include "string.h"

#include "../error_handler/error_handler.h"

char *create_string(int size, int *result_code);

char *copy_string(char *initial_string, int *result_code);

char *copy_const_string(const char *initial_string, int *result_code);

void remove_linebreak(char *string);

void destroy_string(char *string_to_be_destroyed);

#endif