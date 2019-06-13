#ifndef WRITER_H
#define WRITER_H

#include <sys/types.h>
#include "sys/stat.h"
#include <unistd.h>
#include "dirent.h"
#include <fcntl.h>
#include <stdio.h>
#include <signal.h>

#include "../../data_structures/client_list/client_list.h"
#include "../../model/settings/settings.h"
#include "../../helpers/helpers.h"
#include "../../error_handler/error_handler.h"
#include "../../log_handler/log_handler.h"

void do_writers_job(int client_id, Settings *settings);

int write_string_in_chunks(char *string, int length, int buffer_size, int fifo_fd);

int write_file_in_chunks(char *filename, int length, int buffer_size, int fifo_fd, int *result_code);

#endif