#ifndef CHILD_LIST_H
#define CHILD_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "../../error_handler/error_handler.h"
#include "../../string/string_handler.h"

typedef struct ChildListNode
{
    struct ChildListNode *next;
    int id;
    int pid;
    int is_writer;
    int repeated;
} ChildListNode;

ChildListNode *initialize_child_list(int *result_code, int id, int pid, int is_writer);

int add_to_child_list(ChildListNode **start_node, int id, int pid, int is_writer);

int remove_by_pid(ChildListNode **start_node_ptr, int pid);

ChildListNode *get_by_pid(ChildListNode *start_node, int pid);

ChildListNode *get_by_id(ChildListNode *start_node, int id, int is_writer);

void destroy_child_list(ChildListNode *child_list_node);

void destroy_child_list_without_killing(ChildListNode *child_list_node);

void destroy_child_node(ChildListNode *child_list_node);

#endif