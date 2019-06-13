#include "./writer.h"

char *writer_pipe_buffer = NULL;
int writer_pipe_fd = 0;

void writer_kill_handler(int sig)
{
    printf("Process with pid: %d , received killing signal\n", getpid());

    int res = fcntl(writer_pipe_fd, F_GETFL);
    if (res != -1)
    {
        close(writer_pipe_fd);
    }

    if (writer_pipe_buffer != NULL)
    {
        destroy_string(writer_pipe_buffer);
    }

    kill(getppid(), SIGUSR2);
    exit(1);
}

void do_writers_job(int client_id, Settings *settings)
{
    printf("====== Entered in writer of %d for writing to %d\n", settings->id, client_id);

    sigset_t client_sigset;
    sigemptyset(&client_sigset);
    sigaddset(&client_sigset, SIGKILL);

    static struct sigaction act_on_kill;
    act_on_kill.sa_handler = writer_kill_handler;
    sigaction(SIGKILL, &act_on_kill, NULL);

    int total_bytes = 0;
    char *line = NULL;
    size_t length = 0;
    FILE *fp;

    int writer_pipe_fd = 0, result_code = SUCCESS;
    unsigned int file_size = 0;
    char local_buffer[400];
    char file_name_buffer[500];
    writer_pipe_buffer = create_string(settings->buffer_size, &result_code);
    sprintf(local_buffer, "%s/%d_to_%d.fifo", settings->common_dir, settings->id, client_id);

    printf("====== Trying to open pipe: %s\n", local_buffer);

    if ((mkfifo(local_buffer, 0666)) > 0 && (errno != EEXIST))
    {
        printf("Parent pid : %d\n", getppid());
        print_error(COULD_NOT_CREATE_FIFO, "Writer");
        kill(getppid(), SIGUSR2);
        exit(1);
    }

    if ((writer_pipe_fd = open(local_buffer, O_WRONLY)) < 0)
    {
        printf("Parent pid : %d\n", getppid());
        print_error(COULD_NOT_OPEN_FIFO, "Writer");
        kill(getppid(), SIGUSR2);
        exit(1);
    }

    for (int i = 0; i < 2; i++)
    {
        // send dirs
        if (i == 0)
        {
            sprintf(local_buffer, "%s/directory_list", settings->input_dir);
        }
        // send files
        else
        {
            sprintf(local_buffer, "%s/file_list", settings->input_dir);
        }

        printf("====== Opening file %s\n", local_buffer);

        fp = fopen(local_buffer, "r");

        if (fp == NULL)
        {
            print_error(READ_FILE_ERROR, "List opening\n");
            close(writer_pipe_fd);
            kill(getppid(), SIGUSR2);
            exit(1);
        }
        while (getline(&line, &length, fp) != EOF)
        {
            // sending names
            remove_linebreak(line);
            printf("====== Sending name : %s\n", line);
            short name_size = (short)strlen(line);
            printf("====== Size of file name is: %d\n", name_size);

            memcpy(writer_pipe_buffer, &name_size, 2);

            if (write(writer_pipe_fd, writer_pipe_buffer, sizeof(short)) != sizeof(short))
            {
                free(line);
                line = NULL;
                fclose(fp);
                print_error(COULD_NOT_WRITE_TO_FIFO, "Writer to fifo");
                close(writer_pipe_fd);
                kill(getppid(), SIGUSR2);
                exit(1);
            }

            result_code = write_string_in_chunks(line, strlen(line), settings->buffer_size, writer_pipe_fd);
            if (result_code != SUCCESS)
            {
                free(line);
                line = NULL;
                fclose(fp);
                close(writer_pipe_fd);
                kill(getppid(), SIGUSR2);
                exit(1);
            }

            write_stats_to_log(settings->log_file, line, 1, settings->id, client_id, strlen(line) + 2, i == 0 ? 1 : 0);

            // if is dir
            if (i == 0)
            {
                remove_linebreak(line);
                sprintf(file_name_buffer, "%s%s", settings->input_dir, line);
                file_size = 0;
                memcpy(writer_pipe_buffer, &file_size, 4);
                write(writer_pipe_fd, writer_pipe_buffer, 4);
            }
            // if is a file
            else
            {
                remove_linebreak(line);
                sprintf(file_name_buffer, "%s/%s", settings->input_dir, line);
                file_size = get_file_size(file_name_buffer, &result_code);
                if (result_code != SUCCESS)
                {
                    free(line);
                    line = NULL;
                    fclose(fp);
                    printf("The size of a file is :%d \n", file_size);
                    print_error(COULD_NOT_WRITE_TO_FIFO, "The size of a file is too damn high");
                    close(writer_pipe_fd);
                    kill(getppid(), SIGUSR2);
                    exit(1);
                }

                memcpy(writer_pipe_buffer, &file_size, 4);

                printf("====== Writing file size that is : %d\n", file_size);

                if (write(writer_pipe_fd, writer_pipe_buffer, 4) != 4)
                {
                    free(line);
                    line = NULL;
                    fclose(fp);
                    print_error(COULD_NOT_WRITE_TO_FIFO, "Writer to fifo");
                    close(writer_pipe_fd);
                    kill(getppid(), SIGUSR2);
                    exit(1);
                }

                total_bytes = write_file_in_chunks(file_name_buffer, strlen(file_name_buffer), settings->buffer_size, writer_pipe_fd, &result_code);
                if (result_code != SUCCESS)
                {
                    free(line);
                    line = NULL;
                    fclose(fp);
                    close(writer_pipe_fd);
                    kill(getppid(), SIGUSR2);
                    exit(1);
                }
                write_stats_to_log(settings->log_file, file_name_buffer, 1, settings->id, client_id, total_bytes + 4, 0);
            }
        }

        free(line);
        line = NULL;
        fclose(fp);
    }

    writer_pipe_buffer[0] = '0';
    writer_pipe_buffer[1] = '0';

    printf("====== Writing end of communication (00)\n");

    if (write(writer_pipe_fd, writer_pipe_buffer, 2) != 2)
    {
        print_error(COULD_NOT_WRITE_TO_FIFO, "Writer to fifo");
        close(writer_pipe_fd);
        kill(getppid(), SIGUSR2);
        exit(1);
    }

    destroy_string(writer_pipe_buffer);
    result_code = close(writer_pipe_fd);
    if (result_code < 0)
    {
        print_error(COULD_NOT_CLOSE_FILE, "Writing Pipe");
        kill(getppid(), SIGUSR2);
        exit(1);
    }
    printf("====== Sending success signal to parrent (writer)\n");
    kill(getppid(), SIGUSR1);
    return;
}

int write_string_in_chunks(char *string, int length, int buffer_size, int fifo_fd)
{
    int current = 0;
    if (length <= buffer_size)
    {
        printf("====== Writing to fifo %s\n", string);
        if (write(fifo_fd, string, length) != length)
        {
            print_error(COULD_NOT_WRITE_TO_FIFO, "Writer to fifo");
            return COULD_NOT_WRITE_TO_FIFO;
        }
        printf("====== Wrote : %.*s\n", length, string);
        return SUCCESS;
    }
    else
    {
        while (current != length)
        {
            printf("====== Writing to fifo %s (partialy)\n", string);
            if (write(fifo_fd, string + current, buffer_size) != buffer_size)
            {
                print_error(COULD_NOT_WRITE_TO_FIFO, "Writer to fifo");
                return COULD_NOT_WRITE_TO_FIFO;
            }
            current += buffer_size;
            if (length - current <= buffer_size)
            {
                printf("====== Writing to fifo %s (partialy)\n", string);
                if (write(fifo_fd, string + current, length - current) != length - current)
                {
                    print_error(COULD_NOT_WRITE_TO_FIFO, "Writer to fifo");
                    return COULD_NOT_WRITE_TO_FIFO;
                }
                return SUCCESS;
            }
        }
    }
    return SUCCESS;
}

int write_file_in_chunks(char *filename, int length, int buffer_size, int fifo_fd, int *result_code)
{
    int total_count = 0;
    int num_bytes = 0;
    char local_buffer[400];
    int fd = open(filename, O_RDONLY, 0775);
    if (fd < 0)
    {
        print_error(COULD_NOT_OPEN_FILE, "Write file in chunks");
        return COULD_NOT_OPEN_FILE;
    }

    while (1)
    {
        num_bytes = read(fd, local_buffer, buffer_size);
        total_count += num_bytes;
        printf("====== Writing file : %s\n", filename);

        if (write(fifo_fd, local_buffer, num_bytes) != num_bytes)
        {
            print_error(COULD_NOT_WRITE_TO_FIFO, "Writer to fifo");
            return COULD_NOT_WRITE_TO_FIFO;
        }
        if (num_bytes == 0 || num_bytes < buffer_size)
        {
            break;
        }
    }

    *result_code = close(fd);
    if (*result_code < 0)
    {
        print_error(COULD_NOT_CLOSE_FILE, "Writing file in chunks");
        return 0;
    }
    return total_count;
}