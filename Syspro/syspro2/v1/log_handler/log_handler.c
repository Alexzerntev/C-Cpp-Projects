#include "./log_handler.h"

void write_stats_to_log(char *log_file_name, char *file_name, int is_send, int sender_id, int receiver_id, int bytes_count, int is_dir)
{
    char local_buffer[500];
    int result_code;
    int fd = open(log_file_name, O_WRONLY | O_CREAT | O_APPEND, 0755);

    if (fd < 0)
    {
        print_error(COULD_NOT_OPEN_FILE, "Log file");
        return;
    }
    result_code = flock(fd, LOCK_EX);
    if (result_code != 0)
    {
        print_error(COULD_NOT_LOCK_FILE, "Log file");
    }
    sprintf(local_buffer, "%d %d %d %d %s %d\n", is_send, sender_id, receiver_id, bytes_count, file_name, is_dir);
    result_code = write(fd, local_buffer, strlen(local_buffer));
    if (result_code != strlen(local_buffer))
    {
        print_error(COULD_NOT_WRITE_TO_FILE, "Log file");
    }

    result_code = flock(fd, LOCK_UN);
    if (result_code != 0)
    {
        print_error(COULD_NOT_UNLOCK_FILE, "Log file");
        return;
    }
    result_code = close(fd);
    if (result_code == -1)
    {
        printf("Could not close a file : %s\n", log_file_name);
        return;
    }

    return;
}

void write_disconnected(int id, char *log_file_name)
{
    char local_buffer[500];
    int result_code;
    int fd = open(log_file_name, O_WRONLY | O_CREAT | O_APPEND, 0755);
    if (fd < 0)
    {
        print_error(COULD_NOT_OPEN_FILE, "Log file");
        return;
    }
    result_code = flock(fd, LOCK_EX);
    if (result_code != 0)
    {
        print_error(COULD_NOT_LOCK_FILE, "Log file");
    }
    sprintf(local_buffer, "connected %d\n", id);
    result_code = write(fd, local_buffer, strlen(local_buffer));
    if (result_code != strlen(local_buffer))
    {
        print_error(COULD_NOT_WRITE_TO_FILE, "Log file");
    }

    result_code = flock(fd, LOCK_UN);
    if (result_code != 0)
    {
        print_error(COULD_NOT_UNLOCK_FILE, "Log file");
        return;
    }
    result_code = close(fd);
    if (result_code == -1)
    {
        printf("Could not close a file : %s\n", log_file_name);
        return;
    }

    return;
}

void write_connected(int id, char *log_file_name)
{
    char local_buffer[500];
    int result_code;
    int fd = open(log_file_name, O_WRONLY | O_CREAT | O_APPEND, 0755);
    if (fd < 0)
    {
        print_error(COULD_NOT_OPEN_FILE, "Log file");
        return;
    }
    result_code = flock(fd, LOCK_EX);
    if (result_code != 0)
    {
        print_error(COULD_NOT_LOCK_FILE, "Log file");
    }
    sprintf(local_buffer, "disconnected %d\n", id);
    result_code = write(fd, local_buffer, strlen(local_buffer));
    if (result_code != strlen(local_buffer))
    {
        print_error(COULD_NOT_WRITE_TO_FILE, "Log file");
    }

    result_code = flock(fd, LOCK_UN);
    if (result_code != 0)
    {
        print_error(COULD_NOT_UNLOCK_FILE, "Log file");
        return;
    }
    result_code = close(fd);
    if (result_code == -1)
    {
        printf("Could not close a file : %s\n", log_file_name);
        return;
    }

    return;
}