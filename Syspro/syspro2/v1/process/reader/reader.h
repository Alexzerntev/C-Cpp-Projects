#ifndef READER_H
#define READER_H

#include "sys/stat.h"
#include <unistd.h>
#include "dirent.h"
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/signal.h>

#include "../../data_structures/client_list/client_list.h"
#include "../../model/settings/settings.h"
#include "../../error_handler/error_handler.h"
#include "../../log_handler/log_handler.h"

void do_readers_job(int client_id, Settings *settings);

int create_custom_dir(char *path);

int receive_file_name(char *return_buffer, char *fifo_buffer, int buffer_size, int file_size, int fifo_fd);

int receive_file(char *file_name, char *fifo_buffer, int buffer_size, unsigned int file_name_size, int fifo_fd, int *result_code);

#endif