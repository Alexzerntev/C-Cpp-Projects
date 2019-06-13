#ifndef CLIENT_H
#define CLIENT_H

#define LOG_ON 0
#define GET_CLIENTS 1
#define LOG_OFF 2

#define MAX_CONS 100

#define SA struct sockaddr

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/time.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <ifaddrs.h>

#include "../client_settings/client_settings.h"
#include "../../common/error_handler/error_handler.h"
#include "../../common/string/string_handler.h"
#include "../../common/data_structures/cyc_buffer/cyc_buffer.h"
#include "../../common/data_structures/con_list/con_list.h"
#include "../../common/file_manage/file_manage.h"

int connect_to_server(ClientSettings *settings);
void get_clients_from_server(int server_socket);
void disconnect_from_server(int server_socket);
void send_log_on(int sockfd);
int write_string_in_chunks(char *string, int length, int buffer_size, int fifo_fd);
int write_file_in_chunks(char *filename, int length, int fifo_fd, int *result_code);
int create_custom_dir(char *path);
int receive_file(char *file_name, char *fifo_buffer, int buffer_size, unsigned int file_size, int fifo_fd, int *result_code);
void welcoming(ClientSettings *settings);
int connect_to_client(BufferItem *buffer_item);
void setnonblocking(int sock);
void build_select_list();
void handle_new_connection();
void read_socks();
void deal_with_data(int listnum, ClientSettings *client_settings);
void give_file_list(int listnum, ClientSettings *client_settings);
void give_file(int listnum, BufferItem *buffer_item, ClientSettings *settings);
void handle_log_off(int listnum);
int create_custom_dir(char *path);
void write_to_buffer(BufferItem *buffer_item);
void *read_from_buffer(void *arg);
void deal_with_buffer_item(BufferItem *buffer_item);

#endif