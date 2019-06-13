#ifndef STRING_HANDLER_H
#define STRING_HANDLER_H

#include <stdlib.h>
#include "string.h"

#include "../error_handler/error_handler.h"
#include "../date_time/date_time.h"

char *create_string(int size, int *result_code);
char *copy_string(char *initial_string, int *result_code);
char *copy_const_string(const char *initial_string, int *result_code);
char *concat_date_time(const char *str1, const char *str2, int *result_code);

char *add_id_to_transaction(char *line, int id, int *result_code);
char *add_date_time_to_transaction(char *line, int *result_code);

void remove_line_break(char *string_with_line_break);
void remove_semicolon(char *string_with_line_break);

void destroy_string(char *string_to_be_destroyed);

#endif