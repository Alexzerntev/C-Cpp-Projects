#include <stdio.h>
#include <sys/wait.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/signal.h>
#include <unistd.h>
#include "server_settings/server_settings.h"
#include "server/server.h"

ServerSettings *settings = NULL;
extern ConListNode *con_list;
extern pthread_mutex_t mutex;

void handle_end(int sig)
{
    printf("Signal for termination received, terminating...\n");
    pthread_mutex_destroy(&mutex);
    destroy_server_settings(settings);
    destroy_con_list(con_list);
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
    settings = create_server_settings(&result_code);
    if (result_code != SUCCESS)
    {
        return 1;
    }

    result_code = validate_and_store_server_settings(argv, argc, settings);
    if (result_code != SUCCESS)
    {
        return 1;
    }

    initialize_and_operate_server(settings);

    //House keeping
    destroy_server_settings(settings);
    return 0;
}
