#include "intercom_managment.h"

void child_operate(
    int uid,
    int ct_to_execute,
    int checker_sem_ids,
    int data_sem_id,
    shared_memory *data_sm[2],
    shared_memory *checker_sm[2])
{
    int reciever, will_send, ct_executed = 0, flag = 0;
    char *message;
    Container *container = (Container *)malloc(sizeof(Container));
    Checker *checker = (Checker *)malloc(sizeof(Checker));

    while (ct_to_execute > ct_executed)
    {
        char *log_message = (char *)malloc(MAX_LOG_SIZE * sizeof(char)); // log function frees this

        // Recieving part
        down(checker_sem_ids, INPUT);
        memcpy(checker, checker_sm[INPUT]->shared_mem_ptr, sizeof(Checker));
        up(checker_sem_ids, INPUT);

        if (checker->has_items == 1)
        {
            memcpy(container, data_sm[INPUT]->shared_mem_ptr, sizeof(Container));
            sprintf(log_message, "On clock tick %d, Recieved message from procces %d, the message content is: %s",
                    ct_executed, container->sender, container->message);
            write_to_log(log_message, uid);
            down(checker_sem_ids, INPUT);
            checker->has_items = 0;
            memcpy(checker_sm[INPUT]->shared_mem_ptr, checker, sizeof(Checker));
            up(checker_sem_ids, INPUT);
        }

        // Sending part
        will_send = get_send_chance_value(uid);
        if (will_send == 0)
        {
            // Init
            message = get_random_message();
            reciever = get_proccess_id_to_send(uid);

            //Writing to log
            sprintf(log_message, "On clock tick %d, sending to proccess with uid: %d, message: %s",
                    ct_executed, reciever, message);
            write_to_log(log_message, uid);

            // Actual writing
            down(data_sem_id, 0);

            // Recieving part
            down(checker_sem_ids, INPUT);
            memcpy(checker, checker_sm[INPUT]->shared_mem_ptr, sizeof(Checker));
            up(checker_sem_ids, INPUT);

            if (checker->has_items == 1)
            {
                memcpy(container, data_sm[INPUT]->shared_mem_ptr, sizeof(Container));
                sprintf(log_message, "On clock tick %d, Recieved message from procces %d, the message content is: %s",
                        ct_executed, container->sender, container->message);
                write_to_log(log_message, uid);
                down(checker_sem_ids, INPUT);
                checker->has_items = 0;
                memcpy(checker_sm[INPUT]->shared_mem_ptr, checker, sizeof(Checker));
                up(checker_sem_ids, INPUT);
            }

            strcpy(container->message, message);
            container->sender = uid;
            container->reciever = reciever;
            memcpy(data_sm[OUTPUT]->shared_mem_ptr, container, sizeof(Container));
            flag = 1;

            free(message);
        }
        else
        {
            sprintf(log_message, "On clock tick %d, did not send anything \n",
                    ct_executed);
            write_to_log(log_message, uid);
        }

        // Finalizing tick
        ct_executed++;
        // Inform D on ticks status to wait() if finished
        down(checker_sem_ids, OUTPUT);
        memcpy(checker, checker_sm[OUTPUT]->shared_mem_ptr, sizeof(Checker));
        checker->ticks_executed = ct_executed;

        if (flag == 1)
        {
            checker->has_items = 1;
            flag = 0;
        }

        memcpy(checker_sm[OUTPUT]->shared_mem_ptr, checker, sizeof(Checker));
        up(checker_sem_ids, OUTPUT);

        free(log_message);
    }
    free(checker);
    free(container);
    exit(0);
}

void parent_operate(
    int ct,
    shared_memory *data_sm[3][2],
    shared_memory *checker_sm[3][2],
    int checker_sem_ids[3],
    int data_sem_ids[3])
{
    Container *container = (Container *)malloc(sizeof(Container));
    Checker *checker = (Checker *)malloc(sizeof(Checker));
    int terminated_children[3];

    for (int i = 0; i < 3; i++)
    {
        terminated_children[i] = 0;
    }

    Buffer *buffer = (Buffer *)malloc(sizeof(Buffer));
    buffer->current = NULL;
    buffer->head = NULL;
    buffer->last = NULL;

    while (1)
    {
        for (int i = 0; i < 3; i++)
        {
            if (terminated_children[i] == 1)
            {
                continue;
            }

            // Firstly check if proccess has any incoming messages
            down(checker_sem_ids[i], INPUT);
            memcpy(checker, checker_sm[i][INPUT]->shared_mem_ptr, sizeof(Checker));
            up(checker_sem_ids[i], INPUT);

            if (checker->has_items == 0)
            {
                struct node *temp = find(i, buffer);
                if (temp != NULL)
                {
                    memcpy(data_sm[i][INPUT]->shared_mem_ptr, &temp->data, sizeof(Container));
                    down(checker_sem_ids[i], INPUT);
                    checker->has_items = 1;
                    memcpy(checker_sm[i][INPUT]->shared_mem_ptr, checker, sizeof(Checker));
                    up(checker_sem_ids[i], INPUT);
                    free(delete_by_key(i, buffer));
                }
            }

            // Secondly check if the proccess has something to send
            down(checker_sem_ids[i], OUTPUT);
            memcpy(checker, checker_sm[i][OUTPUT]->shared_mem_ptr, sizeof(Checker));
            up(checker_sem_ids[i], OUTPUT);

            // Collecting zombie
            if (checker->ticks_executed == ct)
            {
                wait(NULL);
                terminated_children[i] = 1;

                // Removing packages for the terminated proccsess
                while (delete_by_key(i, buffer) != NULL)
                {
                }

                if (
                    terminated_children[0] == 1 &&
                    terminated_children[1] == 1 &&
                    terminated_children[2] == 1)
                {
                    // Cleaning buffer
                    while (!isEmpty(buffer))
                    {
                        delete_first(buffer);
                    }
                    free(buffer);
                    free(checker);
                    free(container);
                    return;
                }
            }

            if (checker->has_items == 1)
            {
                memcpy(container, data_sm[i][OUTPUT]->shared_mem_ptr, sizeof(Container));
                insert_first(container->reciever, container, buffer);
                down(checker_sem_ids[i], OUTPUT);
                checker->has_items = 0;
                memcpy(checker_sm[i][OUTPUT]->shared_mem_ptr, checker, sizeof(Checker));
                up(checker_sem_ids[i], OUTPUT);
                up(data_sem_ids[i], 0);
            }
        }
    }
}
