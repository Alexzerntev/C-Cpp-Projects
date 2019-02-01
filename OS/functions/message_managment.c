#include "message_managment.h"

int get_proccess_id_to_send(int uid)
{
    return (rand() + uid) % 3;
}

int get_send_chance_value(int uid)
{
    return (rand() + uid) % 3;
}