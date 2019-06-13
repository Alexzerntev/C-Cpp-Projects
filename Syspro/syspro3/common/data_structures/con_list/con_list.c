#include "./con_list.h"

ConListNode *initialize_con_list(int *result_code, BufferItem *buffer_item)
{
    ConListNode *con_list_node = (ConListNode *)malloc(sizeof(ConListNode));
    if (con_list_node == NULL)
    {
        print_error(MEMORY_ALLOCATION_ERROR, "Could not initialize list");
        *result_code = MEMORY_ALLOCATION_ERROR;
        return NULL;
    }

    con_list_node->next = NULL;
    con_list_node->bufer_item = buffer_item;

    *result_code = SUCCESS;
    return con_list_node;
}

int add_to_con_list(ConListNode **start_node, BufferItem *buffer_item)
{
    int result_code = SUCCESS;
    if ((*start_node) == NULL)
    {
        (*start_node) = initialize_con_list(&result_code, buffer_item);
        return result_code;
    }
    ConListNode *to_be_added = (ConListNode *)malloc(sizeof(ConListNode));
    if (to_be_added == NULL)
    {
        print_error(MEMORY_ALLOCATION_ERROR, "Could not initialize list");
        return MEMORY_ALLOCATION_ERROR;
    }
    to_be_added->bufer_item = buffer_item;

    to_be_added->next = *start_node;
    *start_node = to_be_added;

    return SUCCESS;
}

int remove_by_list_num(ConListNode **start_node_ptr, int list_num)
{
    if (get_by_list_num(*start_node_ptr, list_num) == NULL)
    {
        return SUCCESS;
    }

    ConListNode *temp = NULL;
    ConListNode *temp2 = NULL;

    if ((*start_node_ptr) != NULL)
    {
        if ((*start_node_ptr)->bufer_item->list_num == list_num)
        {
            temp = (*start_node_ptr);
            *start_node_ptr = temp->next;
            destroy_con_node(temp);
            temp = NULL;
            return SUCCESS;
        }
    }

    temp = (*start_node_ptr);

    while (temp->next != NULL)
    {
        if (temp->next->bufer_item->list_num == list_num)
        {
            temp2 = temp->next;
            temp->next = temp->next->next;
            destroy_con_node(temp2);
            temp2 = NULL;
            return SUCCESS;
        }
        temp = temp->next;
    }

    return NOT_FOUND;
}

ConListNode *get_remove_by_port_ip(ConListNode **start_node_ptr, BufferItem *buffer_item)
{
    ConListNode *temp = NULL;
    ConListNode *temp2 = NULL;

    if ((*start_node_ptr) != NULL)
    {
        if ((*start_node_ptr)->bufer_item->port == buffer_item->port && (*start_node_ptr)->bufer_item->ip == buffer_item->ip)
        {
            temp = (*start_node_ptr);
            *start_node_ptr = temp->next;
            return temp;
        }
    }

    temp = (*start_node_ptr);

    while (temp->next != NULL)
    {
        if (temp->next->bufer_item->port == buffer_item->port && temp->next->bufer_item->ip == buffer_item->ip)
        {
            temp2 = temp->next;
            temp->next = temp->next->next;
            return temp2;
        }
        temp = temp->next;
    }

    return NULL;
}

ConListNode *get_by_list_num(ConListNode *start_node, int list_num)
{
    ConListNode *temp = start_node;
    while (temp != NULL)
    {
        if (temp->bufer_item->list_num == list_num)
        {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

int exists_ip(ConListNode *start_node, int ip)
{
    if (ip == 0)
    {
        return 1;
    }

    ConListNode *temp = start_node;
    while (temp != NULL)
    {
        if ((unsigned int)temp->bufer_item->ip == ip)
        {
            return 1;
        }
        temp = temp->next;
    }
    return 0;
}

int remove_by_ip_port(ConListNode **start_node_ptr, int ip, int port)
{

    ConListNode *temp = NULL;
    ConListNode *temp2 = NULL;

    if ((*start_node_ptr) != NULL)
    {
        if ((*start_node_ptr)->bufer_item->ip == ip && (*start_node_ptr)->bufer_item->port == port)
        {
            temp = (*start_node_ptr);
            *start_node_ptr = temp->next;
            destroy_con_node(temp);
            temp = NULL;
            return SUCCESS;
        }
    }

    temp = (*start_node_ptr);

    while (temp->next != NULL)
    {
        if (temp->next->bufer_item->ip == ip && temp->next->bufer_item->port == port)
        {
            temp2 = temp->next;
            temp->next = temp->next->next;
            destroy_con_node(temp2);
            temp2 = NULL;
            return SUCCESS;
        }
        temp = temp->next;
    }

    return NOT_FOUND;
}

void destroy_con_list(ConListNode *con_list_node)
{
    ConListNode *temp;
    while (con_list_node != NULL)
    {
        temp = con_list_node;
        con_list_node = con_list_node->next;
        destroy_con_node(temp);
    }
    return;
}

void destroy_con_node(ConListNode *con_list_node)
{
    destroy_cyc_buffer_item(con_list_node->bufer_item, 1);
    free(con_list_node);
}

int get_con_list_size(ConListNode *con_list_node)
{
    ConListNode *temp = con_list_node;
    int count = 0;
    while (temp != NULL)
    {
        count++;
        temp = temp->next;
    }
    return count;
}