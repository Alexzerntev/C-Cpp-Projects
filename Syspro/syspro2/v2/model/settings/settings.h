#ifndef SETTINGS_H
#define SETTINGS_H

#include "../../string/string_handler.h"
#include "../../error_handler/error_handler.h"

typedef struct Settings
{
    int id;
    char *common_dir;
    char *input_dir;
    char *mirror_dir;
    char *log_file;
    int buffer_size;
} Settings;

Settings *create_settings(int *result_code);

int validate_and_store_settings(char **console_input, int size, Settings *settings);

void destroy_settings(Settings *settings);

#endif