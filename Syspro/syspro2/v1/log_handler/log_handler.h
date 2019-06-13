#ifndef LOG_HANDLER_H
#define LOG_HANDLER_H

#include <sys/types.h>
#include "sys/stat.h"
#include <unistd.h>
#include "dirent.h"
#include <fcntl.h>
#include <sys/file.h>

#include "../error_handler/error_handler.h"
#include "../string/string_handler.h"

void write_stats_to_log(char *log_file_name, char *file_name, int is_send, int sender_id, int receiver_id, int bytes_count, int is_dir);
void write_connected(int id, char *log_file_name);
void write_disconnected(int id, char *log_file_name);

#endif