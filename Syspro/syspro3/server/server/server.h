#ifndef SERVER_H
#define SERVER_H

#define LOG_ON 0
#define GET_CLIENTS 1
#define LOG_OFF 2

#define MAX_CONS 10
#define SA struct sockaddr

#include <sys/types.h>  /* sockets */
#include <sys/socket.h> /* sockets */
#include <netinet/in.h> /* Internet sockets */
#include <netdb.h>      /* gethostbyname */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/time.h>
#include <fcntl.h>
#include <arpa/inet.h>

#include "../server_settings/server_settings.h"
#include "../../common/error_handler/error_handler.h"
#include "../../common/string/string_handler.h"
#include "../../common/data_structures/con_list/con_list.h"
#include "../../common/data_structures/cyc_buffer/cyc_buffer.h"

void initialize_and_operate_server(ServerSettings *settings);
int get_command_type(char c, int *term_condition);
void setnonblocking(int sock);
int connect_to_client(BufferItem *buffer_item);
void build_select_list();
void handle_new_connection();
void deal_with_data(int listnum);
void read_socks();
void server_operate(int newsock);
void inform_on_clients(char *string, int list_num);
void inform_off_clients(BufferItem *buffer_item);
void send_user_on(BufferItem *node, BufferItem *client_to_inform);
void send_user_off(BufferItem *node, BufferItem *client_to_inform);
void serve_clients(int list_num);
void send_client(BufferItem *node, int list_num);

#endif