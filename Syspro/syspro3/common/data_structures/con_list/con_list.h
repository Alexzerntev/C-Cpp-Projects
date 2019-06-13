#ifndef CON_LIST_H
#define CON_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "../../error_handler/error_handler.h"
#include "../../string/string_handler.h"
#include "../cyc_buffer/cyc_buffer.h"

typedef struct ConListNode
{
    struct ConListNode *next;
    BufferItem *bufer_item;

} ConListNode;

ConListNode *initialize_con_list(int *result_code, BufferItem *buffer_item);
int add_to_con_list(ConListNode **start_node, BufferItem *buffer_item);
int remove_by_list_num(ConListNode **start_node_ptr, int list_num);
ConListNode *get_by_list_num(ConListNode *start_node, int list_num);
ConListNode *get_remove_by_port_ip(ConListNode **start_node_ptr, BufferItem *buffer_item);
void destroy_con_list(ConListNode *con_list_node);
void destroy_con_node(ConListNode *con_list_node);
int get_con_list_size(ConListNode *con_list_node);
int exists_ip(ConListNode *start_node, int ip);
int remove_by_ip_port(ConListNode **start_node_ptr, int ip, int port);

#endif