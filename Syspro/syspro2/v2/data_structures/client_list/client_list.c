#include "./client_list.h"

ClientListNode *initialize_client_list(int *result_code, int id, int pid, int to_add)
{
    ClientListNode *client_list_node = (ClientListNode *)malloc(sizeof(ClientListNode));
    if (client_list_node == NULL)
    {
        print_error(MEMORY_ALLOCATION_ERROR, "Could not initialize list");
        *result_code = MEMORY_ALLOCATION_ERROR;
        return NULL;
    }

    client_list_node->id = id;
    client_list_node->next = NULL;
    client_list_node->dirty = 1;
    client_list_node->pid = pid;
    client_list_node->to_add = to_add;
    *result_code = SUCCESS;
    return client_list_node;
}

int add_to_client_list(ClientListNode **start_node, int id, int pid, int to_add)
{
    int result_code = SUCCESS;
    if ((*start_node) == NULL)
    {
        (*start_node) = initialize_client_list(&result_code, id, pid, to_add);
        return result_code;
    }

    ClientListNode *to_be_added = (ClientListNode *)malloc(sizeof(ClientListNode));
    if (to_be_added == NULL)
    {
        print_error(MEMORY_ALLOCATION_ERROR, "Could not initialize list");
        return MEMORY_ALLOCATION_ERROR;
    }
    to_be_added->id = id;
    to_be_added->dirty = 1;
    to_be_added->next = *start_node;
    to_be_added->pid = pid;
    to_be_added->to_add = to_add;
    *start_node = to_be_added;

    return SUCCESS;
}

void make_dirty(ClientListNode *start_node, int id)
{
    ClientListNode *temp = start_node;
    while (temp != NULL)
    {
        if (start_node->id == id)
        {
            start_node->dirty = 1;
        }
        temp = temp->next;
    }
    return;
}

void clear_dirty(ClientListNode *start_node)
{
    ClientListNode *temp = start_node;
    while (temp != NULL)
    {
        start_node->dirty = 0;
        temp = temp->next;
    }
    return;
}

ClientListNode *find_by_id(ClientListNode *start_node, int id)
{
    ClientListNode *temp = start_node;
    while (temp != NULL)
    {
        if (temp->id == id)
        {
            return temp;
        }

        temp = temp->next;
    }
    return NULL;
}

ClientListNode *find_by_pid(ClientListNode *start_node, int pid)
{
    ClientListNode *temp = start_node;
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

int remove_not_dirty(ClientListNode **start_node_ptr, Settings *settings)
{
    if ((*start_node_ptr) == NULL)
    {
        return SUCCESS;
    }

    ClientListNode *temp;
    ClientListNode *temp2;
    int pid;

    if ((*start_node_ptr) != NULL)
    {
        while ((*start_node_ptr)->dirty == 0)
        {
            temp = (*start_node_ptr);
            *start_node_ptr = temp->next;
            pid = fork();
            if (pid == 0)
            {
                clean_specific_files(settings, temp->id);
                kill(getppid(), SIGUSR1);
                exit(0);
            }

            destroy_client_node(temp);
            temp = NULL;
            if ((*start_node_ptr) == NULL)
            {
                return SUCCESS;
            }
        }
    }

    temp = (*start_node_ptr);

    while (temp->next != NULL)
    {
        if (temp->next->dirty == 0)
        {
            temp2 = temp->next;
            temp->next = temp->next->next;

            pid = fork();
            if (pid == 0)
            {
                clean_specific_files(settings, temp->id);
                kill(getppid(), SIGUSR1);
                exit(0);
            }

            destroy_client_node(temp2);
            temp2 = NULL;
        }
        temp = temp->next;
    }

    return SUCCESS;
}

void destroy_client_list(ClientListNode *client_list_node)
{
    ClientListNode *temp = client_list_node;
    while (client_list_node != NULL)
    {
        temp = client_list_node;
        client_list_node = client_list_node->next;
        destroy_client_node(temp);
    }
    return;
}

void destroy_client_node(ClientListNode *client_list_node)
{
    free(client_list_node);
}

void clean_specific_files(Settings *settings, int id)
{
    char buffer[200];
    sprintf(buffer, "rm -rf %s/%d", settings->mirror_dir, id);
    system(buffer);
}