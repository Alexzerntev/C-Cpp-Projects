#ifndef HELPERS_H
#define HELPERS_H

#include "sys/stat.h"
#include <sys/types.h>
#include <unistd.h>
#include "dirent.h"
#include <fcntl.h>
#include <stdio.h>

#include "../error_handler/error_handler.h"

int get_nuber_of_digits(int number);

unsigned int get_file_size(char *file_name, int *result_code);

#endif