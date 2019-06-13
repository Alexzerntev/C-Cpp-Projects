#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "sys/stat.h"
#include <sys/types.h>
#include <unistd.h>
#include "dirent.h"
#include <fcntl.h>
#include "../../error_handler/error_handler.h"
#include "../../model/settings/settings.h"
#include "../../data_structures/client_list/client_list.h"

int initialize_dirs(Settings *settings);

int dir_exists(char *dir_name);

int create_dir(char *dir_name);

int map_input_dir(char *dir_name, char *relative_dir, int files_fd, int dirs_fd);

int create_id_file(char *file_name);

ClientListNode *find_client_by_id(ClientListNode *start_node, int id);

ClientListNode *initialize_clients(Settings *settings, int *result_code);

void refresh_clients(ClientListNode **client_list, Settings *settings, int *result_code);

void clean_files(Settings *settings);

// int get_nuber_of_digits(int number);

#endif