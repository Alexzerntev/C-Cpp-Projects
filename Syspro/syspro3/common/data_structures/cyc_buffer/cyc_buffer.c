#include "./cyc_buffer.h"

BufferItem *to_buffer_item(char *string, int is_file, int list_num, int sock_fd, int left)
{
    int result_code = SUCCESS;
    BufferItem *result = NULL;
    char ip_buffer[200] = {0x0};
    char port_buffer[200] = {0x0};
    char path_buffer[200] = {0x0};
    char version_buffer[200] = {0x0};
    int count = 0;
    int phase = 0;
    if (is_file == 1)
    {
        for (int i = index_of(string, '<') + 1; i < index_of(string, '>'); i++)
        {
            if (string[i] == ',')
            {
                count = 0;
                phase++;
                continue;
            }

            if (phase == 0)
            {
                path_buffer[count] = string[i];
                count++;
            }
            if (phase == 1)
            {
                version_buffer[count] = string[i];
                count++;
            }
        }
        result = (BufferItem *)malloc(sizeof(BufferItem));
        result->path = copy_string(path_buffer, &result_code);
        result->version = copy_string(version_buffer, &result_code);
        result->list_num = list_num;
        result->sock_fd = sock_fd;
        result->left = left;
        return result;
    }
    else
    {
        for (int i = index_of(string, '<') + 1; i < index_of(string, '>'); i++)
        {
            if (string[i] == ',')
            {
                count = 0;
                phase++;
                continue;
            }

            if (phase == 0)
            {
                ip_buffer[count] = string[i];
                count++;
            }
            if (phase == 1)
            {
                port_buffer[count] = string[i];
                count++;
            }
        }
        result = (BufferItem *)malloc(sizeof(BufferItem));
        result->ip = ntohl(atoi(ip_buffer));
        result->port = atoi(port_buffer);
        result->path = NULL;
        result->version = NULL;
        result->list_num = list_num;
        result->sock_fd = sock_fd;
        result->left = left;
        return result;
    }
}

void from_buffer_item(BufferItem *buffer_item, char *return_ptr)
{
    if (buffer_item->path == NULL)
    {
        sprintf(return_ptr, "<%u,%d>", htonl((unsigned int)(buffer_item->ip)), buffer_item->port);
    }
    else
    {
        sprintf(return_ptr, "<%u,%d,%s,%s>", htonl((unsigned int)(buffer_item->ip)), buffer_item->port, buffer_item->path, buffer_item->version);
    }
}

CycBuffer *initialize_cyc_buffer(int *result_code, int size)
{
    *result_code = SUCCESS;
    CycBuffer *cyc_buffer = (CycBuffer *)malloc(sizeof(CycBuffer));
    if (cyc_buffer == NULL)
    {
        print_error(MEMORY_ALLOCATION_ERROR, "Could not initialize list");
        *result_code = MEMORY_ALLOCATION_ERROR;
        return NULL;
    }
    cyc_buffer->content = (BufferItem **)malloc(sizeof(BufferItem *) * size);
    for (int i = 0; i < size; i++)
    {
        cyc_buffer->content[i] = NULL;
    }
    cyc_buffer->size = size;
    cyc_buffer->head = 0;
    cyc_buffer->tail = 0;
    cyc_buffer->current_size = 0;
    return cyc_buffer;
}

int add_to_buffer(CycBuffer *cyc_buffer, BufferItem *item)
{
    if (cyc_buffer->current_size == cyc_buffer->size)
    {
        print_error(BUFFER_FULL, "Can not add to buffer");
        return BUFFER_FULL;
    }
    cyc_buffer->content[cyc_buffer->tail] = item;
    cyc_buffer->tail = (cyc_buffer->tail + 1) % cyc_buffer->size;
    cyc_buffer->current_size++;
    return SUCCESS;
}

int is_cyc_buffer_empty(CycBuffer *cyc_buffer)
{
    if (cyc_buffer->current_size == 0)
    {
        return 1;
    }
    return 0;
}

int is_cyc_buffer_full(CycBuffer *cyc_buffer)
{
    if (cyc_buffer->current_size == cyc_buffer->size)
    {
        return 1;
    }
    return 0;
}

// The caller has to delete the item in the end
BufferItem *remove_from_buffer(CycBuffer *cyc_buffer)
{
    if (cyc_buffer->current_size == 0)
    {
        return NULL;
    }

    BufferItem *temp = cyc_buffer->content[cyc_buffer->head];
    cyc_buffer->head = (cyc_buffer->head + 1) % cyc_buffer->size;
    cyc_buffer->current_size--;
    return temp;
}

BufferItem *find_by_fd(CycBuffer *cyc_buffer, int fd)
{
    if (cyc_buffer->current_size == 0)
    {
        return NULL;
    }

    for (int i = cyc_buffer->head; i != (cyc_buffer->tail % cyc_buffer->size); i++)
    {
        if (cyc_buffer->content[i]->sock_fd == fd)
        {
            return cyc_buffer->content[i];
        }
    }

    return NULL;
}

BufferItem *find_by_listnum(CycBuffer *cyc_buffer, int list_num)
{
    if (cyc_buffer->current_size == 0)
    {
        return NULL;
    }

    for (int i = cyc_buffer->head; i != (cyc_buffer->tail % cyc_buffer->size); i++)
    {
        if (cyc_buffer->content[i]->list_num == list_num)
        {
            return cyc_buffer->content[i];
        }
    }

    return NULL;
}

void destroy_cyc_buffer_item(BufferItem *item, int destroy_self)
{
    if (item == NULL)
    {
        return;
    }

    if (item->path != NULL)
    {
        free(item->path);
        item->path = NULL;
    }
    if (item->version != NULL)
    {
        free(item->version);
        item->version = NULL;
    }
    if (destroy_self == 1)
    {
        free(item);
        item = NULL;
    }
    return;
}

void destroy_cyc_buffer(CycBuffer *cyc_buffer)
{
    if (cyc_buffer == NULL)
    {
        return;
    }

    int size = cyc_buffer->size;
    for (int i = 0; i < size; i++)
    {
        destroy_cyc_buffer_item(cyc_buffer->content[i], 1);
    }
    free(cyc_buffer->content);
    free(cyc_buffer);
    return;
}

void mark_left_buffer(CycBuffer *cyc_buffer, int ip, int port)
{
    for (int i = cyc_buffer->head; i != (cyc_buffer->tail % cyc_buffer->size); i++)
    {
        if (cyc_buffer->content[i]->ip == ip && cyc_buffer->content[i]->port == port)
        {
            cyc_buffer->content[i]->left = 1;
        }
    }
}