#ifndef CYC_BUFFER_H
#define CYC_BUFFER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/signal.h>
#include <sys/types.h>  /* sockets */
#include <sys/socket.h> /* sockets */
#include <netinet/in.h> /* Internet sockets */
#include <netdb.h>      /* gethostbyname */
#include <string.h>
#include <pthread.h>
#include <ctype.h>
#include <sys/time.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include "../../error_handler/error_handler.h"
#include "../../string/string_handler.h"

typedef struct
{
    int port;
    in_addr_t ip;
    char *version;
    char *path;
    int list_num;
    int sock_fd;
    int left;
} BufferItem;

typedef struct
{
    int size;
    int head;
    int tail;
    int current_size;
    BufferItem **content;
} CycBuffer;

BufferItem *to_buffer_item(char *string, int is_file, int list_num, int sock_fd, int left);
void from_buffer_item(BufferItem *buffer_item, char *return_ptr);
CycBuffer *initialize_cyc_buffer(int *result_code, int size);
int add_to_buffer(CycBuffer *cyc_buffer, BufferItem *item);
BufferItem *remove_from_buffer(CycBuffer *cyc_buffer);
int is_cyc_buffer_empty(CycBuffer *cyc_buffer);
int is_cyc_buffer_full(CycBuffer *cyc_buffer);
BufferItem *find_by_fd(CycBuffer *cyc_buffer, int fd);
BufferItem *find_by_listnum(CycBuffer *cyc_buffer, int list_num);
void destroy_cyc_buffer_item(BufferItem *item, int destroy_self);
void destroy_cyc_buffer(CycBuffer *cyc_buffer);
void mark_left_buffer(CycBuffer *cyc_buffer, int ip, int port);

#endif