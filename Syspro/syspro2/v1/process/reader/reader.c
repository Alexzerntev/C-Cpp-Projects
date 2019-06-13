#include "./reader.h"

char *reader_pipe_buffer = NULL;
int reader_pipe_fd = 0;

void alarm_handler(int sig)
{
    printf("Process with pid: %d , timed out\n", getpid());
    int res = fcntl(reader_pipe_fd, F_GETFL);
    if (res != -1)
    {
        close(reader_pipe_fd);
    }
    kill(getppid(), SIGUSR2);
    exit(1);
}

void reader_kill_handler(int sig)
{
    printf("Process with pid: %d , received killing signal\n", getpid());

    if (reader_pipe_buffer != NULL)
    {
        destroy_string(reader_pipe_buffer);
    }

    int res = fcntl(reader_pipe_fd, F_GETFL);
    if (res != -1)
    {
        close(reader_pipe_fd);
    }

    kill(getppid(), SIGUSR2);
    exit(1);
}

void do_readers_job(int client_id, Settings *settings)
{
    printf("------ Entered in reader of %d for reading from %d\n", settings->id, client_id);

    sigset_t client_sigset;
    sigemptyset(&client_sigset);
    sigaddset(&client_sigset, SIGALRM);
    sigaddset(&client_sigset, SIGKILL);
    sigprocmask(SIG_UNBLOCK, &client_sigset, NULL);

    static struct sigaction act_on_alarm;
    act_on_alarm.sa_handler = alarm_handler;
    sigaction(SIGALRM, &act_on_alarm, NULL);

    static struct sigaction act_on_kill;
    act_on_kill.sa_handler = reader_kill_handler;
    sigaction(SIGKILL, &act_on_kill, NULL);

    int result_code = SUCCESS, num_bytes = 0, total_bytes = 0, bytes_read_local = 0;
    unsigned int file_size = 0;
    short name_size = 0;
    char local_buffer[300] = {0x0};
    char file_name_buffer[300] = {0x0};
    char temp[500] = {0x0};
    reader_pipe_buffer = create_string(settings->buffer_size, &result_code);
    if (result_code != SUCCESS)
    {
        kill(getppid(), SIGUSR2);
        exit(1);
    }

    sprintf(local_buffer, "%s/%d_to_%d.fifo", settings->common_dir, client_id, settings->id);

    printf("------ Trying to open pipe: %s\n", local_buffer);

    if ((mkfifo(local_buffer, 0666)) > 0 && (errno != EEXIST))
    {
        printf("Parent pid : %d\n", getppid());
        print_error(COULD_NOT_CREATE_FIFO, "Reader");
        destroy_string(reader_pipe_buffer);
        kill(getppid(), SIGUSR2);
        exit(1);
    }

    if ((reader_pipe_fd = open(local_buffer, O_RDONLY)) < 0)
    {
        printf("Parent pid : %d\n", getppid());
        print_error(COULD_NOT_OPEN_FIFO, "Reader");
        destroy_string(reader_pipe_buffer);
        kill(getppid(), SIGUSR2);
        exit(1);
    }

    sprintf(file_name_buffer, "%s/%d%c", settings->mirror_dir, client_id, '\0');
    result_code = create_custom_dir(file_name_buffer);

    if (result_code != SUCCESS)
    {
        printf("Could not create directory : %s\n", file_name_buffer);
        destroy_string(reader_pipe_buffer);
        close(reader_pipe_fd);
        kill(getppid(), SIGUSR2);
        exit(1);
    }

    printf("------ Created dir %s\n", file_name_buffer);

    while (1)
    {
        alarm(30);
        bytes_read_local = 0;

        while (bytes_read_local != 2)
        {
            num_bytes = read(reader_pipe_fd, reader_pipe_buffer + (bytes_read_local * sizeof(char)), 2 - bytes_read_local);
            if (num_bytes < 0)
            {
                print_error(COULD_NOT_READ_FROM_FIFO, "Reading file size");
                destroy_string(reader_pipe_buffer);
                close(reader_pipe_fd);
                kill(getppid(), SIGUSR2);
                exit(1);
            }
            bytes_read_local += num_bytes;
        }

        alarm(0);

        memcpy(&name_size, reader_pipe_buffer, 2);

        if (reader_pipe_buffer[0] == '0' && reader_pipe_buffer[1] == '0')
        {
            break;
        }

        printf("------ Reading filename size which is: %d\n", name_size);

        if (name_size == 0 && strlen(reader_pipe_buffer) != 1)
        {
            print_error(INT_CONVERTION_ERROR, "File name size reading");
            destroy_string(reader_pipe_buffer);
            close(reader_pipe_fd);
            kill(getppid(), SIGUSR2);
            exit(1);
        }

        memset(file_name_buffer, '\0', 300);
        result_code = receive_file_name(file_name_buffer, reader_pipe_buffer, settings->buffer_size, name_size, reader_pipe_fd);

        bytes_read_local = 0;
        while (bytes_read_local != 4)
        {
            num_bytes = read(reader_pipe_fd, reader_pipe_buffer + (bytes_read_local * sizeof(char)), 4 - bytes_read_local);
            if (num_bytes < 0)
            {
                print_error(COULD_NOT_READ_FROM_FIFO, "Reading file size");
                destroy_string(reader_pipe_buffer);
                close(reader_pipe_fd);
                kill(getppid(), SIGUSR2);
                exit(1);
            }
            bytes_read_local += num_bytes;
        }

        memcpy(&file_size, reader_pipe_buffer, 4);

        printf("------ Reading file which size is: %d\n", file_size);

        if (file_name_buffer[0] == '/')
        {
            sprintf(temp, "%s/%d%s", settings->mirror_dir, client_id, file_name_buffer);

            printf("------ The received name is a directory, creating directory: %s\n", temp);

            result_code = create_custom_dir(temp);
            if (result_code != SUCCESS)
            {
                destroy_string(reader_pipe_buffer);
                close(reader_pipe_fd);
                kill(getppid(), SIGUSR2);
                exit(1);
            }
            write_stats_to_log(settings->log_file, file_name_buffer, 0, settings->id, client_id, strlen(file_name_buffer) + 2, 1);
        }
        else
        {
            alarm(30);
            sprintf(temp, "%s/%d/%s", settings->mirror_dir, client_id, file_name_buffer);
            total_bytes = receive_file(temp, reader_pipe_buffer, settings->buffer_size, file_size, reader_pipe_fd, &result_code);
            alarm(0);
            if (result_code != SUCCESS)
            {
                destroy_string(reader_pipe_buffer);
                close(reader_pipe_fd);
                kill(getppid(), SIGUSR2);
                exit(1);
            }
            write_stats_to_log(settings->log_file, file_name_buffer, 0, settings->id, client_id, total_bytes + strlen(file_name_buffer) + 6, 0);
            alarm(0);
        }
    }
    destroy_string(reader_pipe_buffer);
    result_code = close(reader_pipe_fd);
    if (result_code < 0)
    {
        print_error(COULD_NOT_CLOSE_FILE, "Reader");
        kill(getppid(), SIGUSR2);
        exit(1);
    }

    printf("------ Removing fifo file\n");

    sprintf(local_buffer, "rm %s/%d_to_%d.fifo", settings->common_dir, client_id, settings->id);
    system(local_buffer);

    printf("------ Sending success signal to parent (reader)\n");
    kill(getppid(), SIGUSR1);
    return;
}

int create_custom_dir(char *path)
{
    int result_code = mkdir(path, 0755);
    if (result_code < 0)
    {
        printf("Dir creation for path : %s", path);
        print_error(COULD_NOT_CREATE_DIRECTORY, "Mirror for other id");
        return COULD_NOT_CREATE_DIRECTORY;
    }
    return SUCCESS;
}

int receive_file(char *file_name, char *fifo_buffer, int buffer_size, unsigned int file_name_size, int fifo_fd, int *result_code)
{
    int bytes_read, bytes_wrote, total_bytes = 0, bytes_read_local = 0;
    unsigned int count = file_name_size;

    printf("------ Creating and receiving file: %s\n", file_name);

    int file_fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0755);
    if (file_fd < 0)
    {
        print_error(COULD_NOT_OPEN_FILE, "Receiving file");
        printf("File name %s:\n", file_name);
        return 0;
    }
    do
    {
        if (count >= buffer_size)
        {
            bytes_read_local = 0;
            while (bytes_read_local != buffer_size)
            {
                bytes_read = read(fifo_fd, fifo_buffer + (bytes_read_local * sizeof(char)), buffer_size - bytes_read_local);
                if (bytes_read <= buffer_size)
                {
                    bytes_read_local += bytes_read;
                }
                if (bytes_read < 0)
                {
                    print_error(COULD_NOT_READ_FROM_FIFO, "Receiving file");
                    printf("File name %s:\n", file_name);
                    close(file_fd);
                    return 0;
                }
            }

            printf("------ Received %d bytes\n", buffer_size);

            total_bytes += bytes_read_local;

            count = count - buffer_size;
            bytes_wrote = write(file_fd, fifo_buffer, bytes_read);
            if (bytes_wrote != bytes_read)
            {
                print_error(COULD_NOT_WRITE_TO_FILE, "Receiving file");
                printf("File name %s:\n", file_name);
                close(file_fd);
                return 0;
            }
        }
        else
        {
            bytes_read_local = 0;
            while (bytes_read_local != count)
            {
                bytes_read = read(fifo_fd, fifo_buffer + (bytes_read_local * sizeof(char)), count - bytes_read_local);
                if (bytes_read < 0)
                {
                    print_error(COULD_NOT_READ_FROM_FIFO, "Receiving file");
                    printf("File name %s:\n", file_name);
                    close(file_fd);
                    return 0;
                }
                if (bytes_read <= count)
                {
                    bytes_read_local += bytes_read;
                }
            }

            printf("------ Received %d bytes\n", count);

            total_bytes += bytes_read_local;
            count = 0;
            bytes_wrote = write(file_fd, fifo_buffer, bytes_read);
            if (bytes_wrote != bytes_read)
            {
                print_error(COULD_NOT_WRITE_TO_FILE, "Receiving file");
                printf("File name %s:\n", file_name);
                close(file_fd);
                return 0;
            }
        }
    } while (count != 0);
    *result_code = close(file_fd);
    if (*result_code < 0)
    {
        print_error(COULD_NOT_CLOSE_FILE, "Receiving file");
        return 0;
    }

    printf("------ Received file: %s successfuly\n", file_name);
    return total_bytes;
}

int receive_file_name(char *return_buffer, char *fifo_buffer, int buffer_size, int file_size, int fifo_fd)
{
    int bytes_read = 0, bytes_wrote = 0, bytes_read_local;
    int count = file_size;
    do
    {
        if (count >= buffer_size)
        {
            bytes_read_local = 0;
            while (bytes_read_local != buffer_size)
            {
                bytes_read = read(fifo_fd, fifo_buffer + (bytes_read_local * sizeof(char)), buffer_size - bytes_read_local);
                printf("------ Received %d bytes out of %d\n", bytes_read, file_size);
                if (bytes_read < 0)
                {
                    print_error(COULD_NOT_READ_FROM_FIFO, "Receiving file name");
                    return COULD_NOT_READ_FROM_FIFO;
                }
                if (bytes_read <= buffer_size)
                {
                    bytes_read_local += bytes_read;
                }
            }

            count = count - buffer_size;
            sprintf(return_buffer + bytes_wrote, "%s", fifo_buffer);
            bytes_wrote += bytes_read;
        }
        else
        {
            bytes_read_local = 0;
            while (bytes_read_local != count)
            {
                bytes_read = read(fifo_fd, fifo_buffer + (sizeof(char) * bytes_read_local), count - bytes_read_local);
                printf("------ Received %d bytes out of %d\n", bytes_read, count);

                if (bytes_read < 0)
                {
                    print_error(COULD_NOT_READ_FROM_FIFO, "Receiving file name");
                    return COULD_NOT_READ_FROM_FIFO;
                }
                if (bytes_read <= count)
                {
                    bytes_read_local += bytes_read;
                }
            }

            count = 0;
            sprintf(return_buffer + bytes_wrote, "%s", fifo_buffer);
            bytes_wrote += bytes_read;
        }
    } while (count != 0);

    return_buffer[bytes_wrote] = '\0';

    printf("------ Received file_name: %s successfuly\n", return_buffer);
    return SUCCESS;
}