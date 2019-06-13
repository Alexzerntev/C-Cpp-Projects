#include "./child_list.h"

ChildListNode *initialize_child_list(int *result_code, int id, int pid, int is_writer)
{
    ChildListNode *child_list_node = (ChildListNode *)malloc(sizeof(ChildListNode));
    if (child_list_node == NULL)
    {
        print_error(MEMORY_ALLOCATION_ERROR, "Could not initialize list");
        *result_code = MEMORY_ALLOCATION_ERROR;
        return NULL;
    }

    child_list_node->id = id;
    child_list_node->next = NULL;
    child_list_node->pid = pid;
    child_list_node->is_writer = is_writer;
    child_list_node->repeated = 0;
    *result_code = SUCCESS;
    return child_list_node;
}

int add_to_child_list(ChildListNode **start_node, int id, int pid, int is_writer)
{
    int result_code = SUCCESS;
    if ((*start_node) == NULL)
    {
        (*start_node) = initialize_child_list(&result_code, id, pid, is_writer);
        return result_code;
    }
    ChildListNode *to_be_added = (ChildListNode *)malloc(sizeof(ChildListNode));
    if (to_be_added == NULL)
    {
        print_error(MEMORY_ALLOCATION_ERROR, "Could not initialize list");
        return MEMORY_ALLOCATION_ERROR;
    }
    to_be_added->id = id;
    to_be_added->pid = pid;
    to_be_added->is_writer = is_writer;
    to_be_added->repeated = 0;

    to_be_added->next = *start_node;
    *start_node = to_be_added;

    return SUCCESS;
}

int remove_by_pid(ChildListNode **start_node_ptr, int pid)
{
    if (get_by_pid(*start_node_ptr, pid) == NULL)
    {
        return SUCCESS;
    }

    ChildListNode *temp = NULL;
    ChildListNode *temp2 = NULL;

    if ((*start_node_ptr) != NULL)
    {
        if ((*start_node_ptr)->pid == pid)
        {
            temp = (*start_node_ptr);
            *start_node_ptr = temp->next;
            destroy_child_node(temp);
            temp = NULL;
            return SUCCESS;
        }
    }

    temp = (*start_node_ptr);

    while (temp->next != NULL)
    {
        if (temp->next->pid == pid)
        {
            temp2 = temp->next;
            temp->next = temp->next->next;
            destroy_child_node(temp2);
            temp2 = NULL;
            return SUCCESS;
        }
        temp = temp->next;
    }

    return NOT_FOUND;
}

ChildListNode *get_by_pid(ChildListNode *start_node, int pid)
{
    ChildListNode *temp = start_node;
    while (temp != NULL)
    {
        if (temp->pid == pid)
        {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

ChildListNode *get_by_id(ChildListNode *start_node, int id, int is_writer)
{
    ChildListNode *temp = start_node;
    while (temp != NULL)
    {
        if (temp->id == id && temp->is_writer == is_writer)
        {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

void destroy_child_list(ChildListNode *child_list_node)
{
    ChildListNode *temp;
    while (child_list_node != NULL)
    {
        temp = child_list_node;
        child_list_node = child_list_node->next;
        printf("Killing child with is_writer: %d and pid : %d\n", temp->is_writer, temp->pid);
        kill(temp->pid, SIGKILL);
        destroy_child_node(temp);
    }
    return;
}

void destroy_child_list_without_killing(ChildListNode *child_list_node)
{
    ChildListNode *temp;
    while (child_list_node != NULL)
    {
        temp = child_list_node;
        child_list_node = child_list_node->next;
        destroy_child_node(temp);
    }
    return;
}

void destroy_child_node(ChildListNode *child_list_node)
{
    free(child_list_node);
}