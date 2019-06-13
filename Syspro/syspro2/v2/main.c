#include <stdio.h>
#include <sys/wait.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <unistd.h>
#include "model/settings/settings.h"
#include "io/file_manager/file_manager.h"
#include "process/reader/reader.h"
#include "process/writer/writer.h"
#include "data_structures/child_list/child_list.h"
#include "data_structures/client_list/client_list.h"
#include "crypto/crypto.h"

static ChildListNode *child_list = NULL;
ClientListNode *client_list = NULL;
Settings *settings = NULL;

void handle_success(int sig, siginfo_t *info, void *ucontext)
{
    int status;
    printf("Child with pid : %d termineted successfully ======\n", info->si_pid);
    waitpid(info->si_pid, &status, WUNTRACED);
    remove_by_pid(&child_list, info->si_pid);
}

void handle_failure(int sig, siginfo_t *info, void *ucontext)
{
    int result_code;
    int status;
    int pid;
    char buf[500];
    ChildListNode *node = get_by_pid(child_list, info->si_pid);

    ChildListNode *node2;

    ClientListNode *client = NULL;

    if (node == NULL)
    {
        client = find_by_pid(client_list, info->si_pid);

        node2 = get_by_id(child_list, client->id, 1);
        if (node2 != NULL)
        {
            kill(node2->pid, SIGKILL);
            remove_by_pid(&child_list, node2->pid);
        }

        node2 = get_by_id(child_list, client->id, 0);
        if (node2 != NULL)
        {
            kill(node2->pid, SIGKILL);
            remove_by_pid(&child_list, node2->pid);
        }

        for (int i = 0; i < 2; i++)
        {
            // critical section
            pid = fork();
            if (pid == 0 && i == 0)
            {
                // do writer stuff
                do_writers_job(client->id, settings);
                destroy_client_list(client_list);
                destroy_child_list_without_killing(child_list);
                destroy_settings(settings);
                exit(0);
            }
            else if (pid != 0 && i == 0)
            {
                // parrent
                result_code = add_to_child_list(&child_list, client->id, pid, 1);
                if (result_code != SUCCESS)
                {
                    print_error(result_code, "Adding child to list (writer)");
                }
            }

            if (pid == 0 && i == 1)
            {
                // do reader stuff
                do_readers_job(client->id, settings);
                destroy_client_list(client_list);
                destroy_child_list_without_killing(child_list);
                destroy_settings(settings);
                exit(0);
            }
            else if (pid != 0 && i == 1)
            {
                // parrent
                result_code = add_to_child_list(&child_list, client->id, pid, 0);
                if (result_code != SUCCESS)
                {
                    print_error(result_code, "Adding child to list (reader)");
                }
            }
            // restore from critical section
        }
        return;
    }

    result_code = waitpid(info->si_pid, &status, WUNTRACED);
    printf("Child with pid : %d had a failure ======\n", info->si_pid);

    if (node->repeated == 2)
    {
        result_code = remove_by_pid(&child_list, info->si_pid);
        if (result_code != SUCCESS)
        {
            printf("Could not remove pid %d from child list\n", info->si_pid);
        }
        return;
    }
    else
    {
        int repeated = node->repeated;

        if (node->is_writer == 0)
        {
            sprintf(buf, "rm -f %s/%d_to_%d.fifo", settings->common_dir, settings->id, node->id);
            system(buf);
        }
        else
        {
            sprintf(buf, "rm -f %s/%d_to_%d.fifo", settings->common_dir, node->id, settings->id);
            system(buf);
        }

        client = find_by_id(client_list, node->id);

        node2 = get_by_id(child_list, client->id, !(node->is_writer));
        if (node2 != NULL)
        {
            kill(node2->pid, SIGKILL);
            remove_by_pid(&child_list, node2->pid);
            remove_by_pid(&child_list, node->pid);
        }

        for (int i = 0; i < 2; i++)
        {
            // critical section
            pid = fork();
            if (pid == 0 && i == 0)
            {
                // do writer stuff
                do_writers_job(client->id, settings);
                destroy_client_list(client_list);
                destroy_child_list_without_killing(child_list);
                destroy_settings(settings);
                exit(0);
            }
            else if (pid != 0 && i == 0)
            {
                // parrent
                result_code = add_to_child_list(&child_list, client->id, pid, 1);
                if (result_code != SUCCESS)
                {
                    print_error(result_code, "Adding child to list (writer)");
                }
                ChildListNode *temp = get_by_pid(child_list, pid);
                temp->repeated = repeated;
            }

            if (pid == 0 && i == 1)
            {
                // do reader stuff
                do_readers_job(client->id, settings);
                destroy_client_list(client_list);
                destroy_child_list_without_killing(child_list);
                destroy_settings(settings);
                exit(0);
            }
            else if (pid != 0 && i == 1)
            {
                // parrent
                result_code = add_to_child_list(&child_list, client->id, pid, 0);
                if (result_code != SUCCESS)
                {
                    print_error(result_code, "Adding child to list (reader)");
                }
                ChildListNode *temp = get_by_pid(child_list, pid);
                temp->repeated = repeated;
            }
            // restore from critical section
        }
        kill(client->pid, SIGUSR2);
    }
}

void handle_end(int sig)
{
    printf("Signal for termination received, terminating...");
    write_disconnected(settings->id, settings->log_file);
    // house keeping
    printf("Removing keys...\n");
    remove_keys(settings->id);

    destroy_client_list(client_list);
    destroy_child_list(child_list);
    clean_files(settings);
    destroy_settings(settings);
    exit(0);
}

int main(int argc, char *argv[])
{
    // Initializing the app
    // Signals setting
    sigset_t blocking_set;
    sigfillset(&blocking_set);

    sigset_t client_sigset;
    sigemptyset(&client_sigset);
    sigaddset(&client_sigset, SIGUSR1);
    sigaddset(&client_sigset, SIGUSR2);
    sigaddset(&client_sigset, SIGQUIT);
    sigaddset(&client_sigset, SIGINT);
    sigprocmask(SIG_UNBLOCK, &client_sigset, NULL);

    static struct sigaction act_on_success;
    act_on_success.sa_handler = (void (*)(int))handle_success;
    act_on_success.sa_flags = SA_SIGINFO;
    sigfillset(&(act_on_success.sa_mask));
    sigaction(SIGUSR1, &act_on_success, NULL);

    static struct sigaction act_on_failure;
    act_on_failure.sa_handler = (void (*)(int))handle_failure;
    act_on_failure.sa_flags = SA_SIGINFO;
    sigfillset(&(act_on_failure.sa_mask));
    sigaction(SIGUSR2, &act_on_failure, NULL);

    static struct sigaction act_on_quit;
    act_on_quit.sa_handler = (void (*)(int))handle_end;
    sigfillset(&(act_on_quit.sa_mask));
    sigaction(SIGQUIT, &act_on_quit, NULL);

    static struct sigaction act_on_int;
    act_on_int.sa_handler = (void (*)(int))handle_end;
    sigfillset(&(act_on_int.sa_mask));
    sigaction(SIGINT, &act_on_int, NULL);

    // Settings creation
    int result_code = SUCCESS;
    int pid;
    settings = create_settings(&result_code);
    if (result_code != SUCCESS)
    {
        destroy_settings(settings);
        exit(1);
    }

    // Create settings struct with the input arguments
    result_code = validate_and_store_settings(argv, argc, settings);
    if (result_code != SUCCESS)
    {
        destroy_settings(settings);
        exit(1);
    }

    // Initialize client directories
    result_code = initialize_dirs(settings);
    if (result_code != SUCCESS)
    {
        destroy_settings(settings);
        exit(1);
    }

    char buffer[200] = {0x0};

    // Map input directory
    int files_fd = -1, dirs_fd = -1;
    sprintf(buffer, "%s/%s", settings->input_dir, "file_list");
    files_fd = open(buffer, O_WRONLY | O_CREAT | O_TRUNC, 0755);
    sprintf(buffer, "%s/%s", settings->input_dir, "directory_list");
    dirs_fd = open(buffer, O_WRONLY | O_CREAT | O_TRUNC, 0755);
    if (files_fd == -1 || dirs_fd == -1)
    {
        clean_files(settings);
        destroy_settings(settings);
        printf("Could not open map files\n");
        exit(1);
    }
    result_code = map_input_dir(settings->input_dir, "", files_fd, dirs_fd);
    if (result_code != SUCCESS)
    {
        clean_files(settings);
        destroy_settings(settings);
        exit(1);
    }
    result_code = close(files_fd);
    if (result_code < 0)
    {
        clean_files(settings);
        print_error(COULD_NOT_CLOSE_FILE, "file_list file");
        destroy_settings(settings);
        exit(1);
    }
    result_code = close(dirs_fd);
    if (result_code < 0)
    {
        clean_files(settings);
        print_error(COULD_NOT_CLOSE_FILE, "directory_list file");
        destroy_settings(settings);
        exit(1);
    }

    // Create common folder entry
    sprintf(buffer, "%s/%d.id", settings->common_dir, settings->id);

    sigprocmask(SIG_SETMASK, &blocking_set, NULL);
    result_code = create_id_file(buffer);

    printf("Generaiting key...\n");
    generate_key(buffer, settings);

    write_connected(settings->id, settings->log_file);
    if (result_code != SUCCESS)
    {
        clean_files(settings);
        destroy_settings(settings);
        exit(1);
    }

    // Create list of clients
    client_list = initialize_clients(settings, &result_code);
    ClientListNode *temp = client_list;

    while (1)
    {
        while (temp != NULL)
        {
            sigprocmask(SIG_SETMASK, &blocking_set, NULL);
            for (int i = 0; i < 2; i++)
            {
                if (temp->to_add == 1)
                {
                    // critical section
                    pid = fork();
                    if (pid == 0 && i == 0)
                    {
                        // do writer stuff
                        do_writers_job(temp->id, settings);
                        destroy_client_list(client_list);
                        destroy_child_list_without_killing(child_list);
                        destroy_settings(settings);
                        exit(0);
                    }
                    else if (pid != 0 && i == 0)
                    {
                        // parrent
                        result_code = add_to_child_list(&child_list, temp->id, pid, 1);
                        if (result_code != SUCCESS)
                        {
                            print_error(result_code, "Adding child to list (writer)");
                        }
                    }

                    if (pid == 0 && i == 1 && temp->to_add == 1)
                    {
                        // do reader stuff
                        do_readers_job(temp->id, settings);
                        destroy_client_list(client_list);
                        destroy_child_list_without_killing(child_list);
                        destroy_settings(settings);
                        exit(0);
                    }
                    else if (pid != 0 && i == 1)
                    {
                        // parrent
                        temp->to_add = 0;
                        result_code = add_to_child_list(&child_list, temp->id, pid, 0);
                        if (result_code != SUCCESS)
                        {
                            print_error(result_code, "Adding child to list (reader)");
                        }
                    }
                    // restore from critical section
                }
            }
            temp = temp->next;
        }

        clear_dirty(client_list);

        sleep(1);
        refresh_clients(&client_list, settings, &result_code);

        // restore from critical section
        sigprocmask(SIG_UNBLOCK, &client_sigset, NULL);

        temp = client_list;
        if (result_code != SUCCESS)
        {
            print_error(result_code, "Failed to refresh client");
            break;
        }
        result_code = remove_not_dirty(&client_list, settings);
        if (result_code != SUCCESS)
        {
            print_error(result_code, "Failed to remove dirty");
            break;
        }
    }
}
