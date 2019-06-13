#ifndef SERVER_SETTINGS_H
#define SERVER_SETTINGS_H

#include "../../common/error_handler/error_handler.h"
#include "../../common/string/string_handler.h"

typedef struct ServerSettings
{
    int port_num;
} ServerSettings;

ServerSettings *create_server_settings(int *result_code);

int validate_and_store_server_settings(char **console_input, int size, ServerSettings *settings);

void destroy_server_settings(ServerSettings *settings);

#endif