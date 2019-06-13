#ifndef CLIENT_LIST_H
#define CLIENT_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/signal.h>
#include "../../error_handler/error_handler.h"
#include "../../string/string_handler.h"
#include "../../model/settings/settings.h"

typedef struct ClientListNode
{
    struct ClientListNode *next;
    int id;
    int pid;
    int dirty;
    int to_add;
} ClientListNode;

ClientListNode *initialize_client_list(int *result_code, int id, int pid, int to_add);

ClientListNode *find_by_id(ClientListNode *start_node, int id);

ClientListNode *find_by_pid(ClientListNode *start_node, int pid);

int add_to_client_list(ClientListNode **start_node, int id, int pid, int to_add);

void clear_dirty(ClientListNode *start_node);

void make_dirty(ClientListNode *start_node, int id);

int remove_not_dirty(ClientListNode **start_node_ptr, Settings *settings);

void clean_specific_files(Settings *settings, int id);

void destroy_client_list(ClientListNode *client_list_node);

void destroy_client_node(ClientListNode *client_list_node);

#endif