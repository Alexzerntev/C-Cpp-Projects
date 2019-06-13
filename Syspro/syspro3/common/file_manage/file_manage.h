#ifndef FILE_MANAGE_H
#define FILE_MANAGE_H

#include <openssl/md5.h>
#include <string.h>
#include <stdio.h>
#include "unistd.h"
#include <dirent.h>
#include "sys/stat.h"
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <libgen.h>

#include "../string/string_handler.h"

char *get_hash_of_file(char *filename);
int check_if_same_version(char *file_path, char *version);
int file_exists(char *file_path);
int dir_exists(char *dir_name);
int create_custom_dir(char *path);
unsigned int get_file_size(char *file_name, int *result_code);
int map_input_dir(char *dir_name, char *relative_dir, int files_fd);
int get_line_count(char *filename);
void create_recursive_dir(char *initial_dir);

#endif