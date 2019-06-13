#ifndef SETTINGS_H
#define SETTINGS_H

#include "../../common/string/string_handler.h"
#include "../../common/error_handler/error_handler.h"

typedef struct ClientSettings
{
    char *dir_name;
    char *server_ip;
    int port_num;
    int worker_threads;
    int buffer_size;
    int server_port;
} ClientSettings;

ClientSettings *create_client_settings(int *result_code);

int validate_and_store_client_settings(char **console_input, int size, ClientSettings *settings);

void destroy_client_settings(ClientSettings *settings);

#endif