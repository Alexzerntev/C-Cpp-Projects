#include <stdio.h>
#include <sys/wait.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/signal.h>
#include <unistd.h>
#include "client_settings/client_settings.h"
#include "./client/client.h"
#include "../common/file_manage/file_manage.h"
#include "../common/string/string_handler.h"

ClientSettings *global_settings = NULL;
int server_fd = -1;
pthread_t *threads;

extern CycBuffer *cyc_buffer;
extern ConListNode *con_list;
extern int exit_requested;
extern pthread_cond_t empty;
extern pthread_cond_t full;
extern pthread_mutex_t lock;

void handle_end(int sig)
{
    printf("Signal for termination received, terminating...\n");

    exit_requested = 1;
    pthread_cond_broadcast(&empty);
    pthread_cond_broadcast(&full);
    for (int i = 0; i < global_settings->worker_threads; i++)
    {
        pthread_join(threads[i], NULL);
        printf("Collected a thread \n");
    }

    pthread_cond_destroy(&empty);
    pthread_cond_destroy(&full);
    pthread_mutex_destroy(&lock);

    int fd = connect_to_server(global_settings);
    disconnect_from_server(fd);
    destroy_con_list(con_list);
    destroy_cyc_buffer(cyc_buffer);

    // house keeping
    destroy_client_settings(global_settings);
    free(threads);
    exit(0);
}

int main(int argc, char *argv[])
{
    sigset_t blocking_set;
    sigfillset(&blocking_set);

    sigset_t client_sigset;
    sigemptyset(&client_sigset);
    sigaddset(&client_sigset, SIGINT);
    sigprocmask(SIG_UNBLOCK, &client_sigset, NULL);

    static struct sigaction act_on_int;
    act_on_int.sa_handler = (void (*)(int))handle_end;
    sigfillset(&(act_on_int.sa_mask));
    sigaction(SIGINT, &act_on_int, NULL);

    int result_code = SUCCESS;
    char buffer[1024];
    global_settings = create_client_settings(&result_code);
    if (result_code != SUCCESS)
    {
        return 1;
    }

    result_code = validate_and_store_client_settings(argv, argc, global_settings);
    if (result_code != SUCCESS)
    {
        return 1;
    }

    cyc_buffer = initialize_cyc_buffer(&result_code, global_settings->buffer_size);

    // Map input directory
    int files_fd = -1;
    sprintf(buffer, "%s/%s", global_settings->dir_name, "file_list");
    files_fd = open(buffer, O_WRONLY | O_CREAT | O_TRUNC, 0755);

    if (files_fd == -1)
    {
        destroy_client_settings(global_settings);
        printf("Could not open map files\n");
        exit(1);
    }

    result_code = map_input_dir(global_settings->dir_name, "", files_fd);
    if (result_code != SUCCESS)
    {
        destroy_client_settings(global_settings);
        exit(1);
    }
    result_code = close(files_fd);
    if (result_code < 0)
    {
        destroy_client_settings(global_settings);
        print_error(COULD_NOT_CLOSE_FILE, "file_list file");
        exit(1);
    }
    char buf[200];
    sprintf(buf, "./remote_files");
    int exists = dir_exists(buf);
    if (exists == 0)
    {
        create_custom_dir(buf);
    }

    server_fd = connect_to_server(global_settings);
    if (server_fd == -1)
    {
        exit(1);
    }

    send_log_on(server_fd);

    get_clients_from_server(server_fd);

    // Thread management
    threads = (pthread_t *)malloc(sizeof(pthread_t) * global_settings->worker_threads);
    if (threads == NULL)
    {
        print_error(MEMORY_ALLOCATION_ERROR, "thread array creation");
        exit(1);
    }

    int err;
    for (int i = 0; i < global_settings->worker_threads; i++)
    {
        err = pthread_create(&threads[i], NULL, read_from_buffer, NULL);
        if (err)
        { /* Create a thread */
            perror("pthread_create");
            exit(1);
        }
        printf("Created a thread \n");
    }

    welcoming(global_settings);

    // house keeping
    return 0;
}
