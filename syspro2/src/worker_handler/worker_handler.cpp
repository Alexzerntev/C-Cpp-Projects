#include "../../include/worker_handler/worker_handler.hpp"

int a;
ResultList **auditor_snapshot;
int global_num_wrokers;
bool signal_in_use = false;
bool alarm_in_use = false;
int killed_pid = -1;

WorkerHandler::WorkerHandler(int num_workers, char *file_path)
{
    this->num_workers = num_workers;
    this->self_maps = NULL;

    this->file_path = new char[strlen(file_path) + 1];
    if (this->file_path == NULL)
    {
        printf("errno = % d \ n ", errno);
        perror("malloc");
    }
    strncpy(this->file_path, file_path, strlen(file_path));
    this->file_path[strlen(file_path)] = '\0';

    this->lines_count = 0;
    this->find_lines_count();

    this->pipe_message_buffer = new char[BUFSIZE];
    if (this->pipe_message_buffer == NULL)
    {
        printf("errno = % d \ n ", errno);
        perror("malloc");
    }
    this->fifo_message_buffer = new char[BUFSIZE];
    if (this->fifo_message_buffer == NULL)
    {
        printf("errno = % d \ n ", errno);
        perror("malloc");
    }

    if (mkfifo(NAMED_PIPE_FILE_NAME, 0600) == -1)
    {
        if (errno != EEXIST)
        {
            perror("mkfifo");
            exit(6);
        };
    }

    this->worker_pipes = new int *[this->num_workers];
    if (this->worker_pipes == NULL)
    {
        printf("errno = % d \ n ", errno);
        perror("malloc");
    }
    this->auditor = new ResultList *[this->num_workers];
    if (this->auditor == NULL)
    {
        printf("errno = % d \ n ", errno);
        perror("malloc");
    }
    auditor_snapshot = this->auditor;
    global_num_wrokers = num_workers;
    for (int i = 0; i < this->num_workers; i++)
    {
        this->worker_pipes[i] = new int[2]; //dont forget to delete
        if (this->worker_pipes[i] == NULL)
        {
            printf("errno = % d \ n ", errno);
            perror("malloc");
        }
        this->auditor[i] = new ResultList();
        if (this->auditor[i] == NULL)
        {
            printf("errno = % d \ n ", errno);
            perror("malloc");
        }
    }

    for (int i = 0; i < this->num_workers; i++)
    {
        if (pipe(this->worker_pipes[i]) == -1)
        {
            perror("pipe call");
            exit(1);
        }
    }
}

WorkerHandler::~WorkerHandler()
{
    //delete[](this->worker_ids);
    //delete[](this->file_path);
    //delete[](this->message_buffer);
    // for (int i = 0; i < this->num_workers; i++)
    // {
    //     delete[](this->worker_pipes[i]);
    // }
    // delete[](this->worker_pipes);
}

void WorkerHandler::initialize(UserInteraction *user_interaction)
{
    signal(SIGCHLD, signal_handler);

    this->user_interaction = user_interaction;
    if (this->lines_count < this->num_workers)
    {
        this->num_workers = lines_count;
        printf("\nWarning, file paths are less than Worker number,"
               "so the programm will create only as many Workers as paths are\n");
    }

    for (int i = 0; i < this->num_workers; i++)
    {
        int pid = fork();

        a = i;

        // child proccesses action
        if (pid == 0)
        {
            this->initialize_worker(i);
        }
        this->auditor[i]->set_pid(pid);
    }

    // Parent proccess actions
    for (int j = 0; j < this->num_workers; j++)
    {
        close(this->worker_pipes[j][0]);
    }

    if ((this->fifo_file_desc = open(NAMED_PIPE_FILE_NAME, O_RDONLY)) < 0)
    {
        perror("fifo open problem");
        exit(3);
    }

    this->distribute_file_paths();

    // for (int i = 0; i < num_workers; i++)
    // {
    //     auditor[i]->next_path->print();
    // } //to be deleted

    while (this->order_action(this->user_interaction->interact()) != 0)
    {
        if (killed_pid != -1)
        {
            int pid = fork();

            // child proccesses action
            if (pid == 0)
            {
                this->initialize_worker(-1);
            }
            killed_pid = -1;
            for (int i = 0; i < num_workers; i++)
            {
                if (this->auditor[i]->get_pid() == killed_pid)
                {
                    this->auditor[i]->set_pid(pid);
                }
            }
        }
    }

    signal_in_use = false;
    //this->order_action("/exit");

    close(this->fifo_file_desc);
    return;
}

void WorkerHandler::find_lines_count()
{
    FILE *file = fopen(this->file_path, "r");
    char *buffer = NULL;
    size_t length;
    if (file == NULL)
    {
        printf("\nError opening file? \n");
        return;
    }

    while (getline(&buffer, &length, file) != EOF)
    {
        this->lines_count++;
    }

    fclose(file);

    return;
}

void WorkerHandler::distribute_file_paths()
{
    FILE *file = fopen(this->file_path, "r");
    if (file == NULL)
    {
        printf("\nError while loading document lines \n");
        return;
    }
    size_t length;
    char *buffer = NULL;
    int current_child = 0;
    while (getline(&buffer, &length, file) != EOF)
    {
        sprintf(this->pipe_message_buffer, "/path_distro %s", buffer);
        this->auditor[current_child]->add_path(this->pipe_message_buffer);
        write(this->worker_pipes[current_child][WRITE], this->pipe_message_buffer, MSGSIZE);
        if (current_child == this->num_workers - 1)
        {
            current_child = 0;
            this->receive_result(-1);
        }
        else
        {
            current_child++;
        }
    }
    fclose(file);
}

int WorkerHandler::order_action(char *order_string)
{
    for (int j = 0; j < this->num_workers; j++)
    {
        strcpy(this->pipe_message_buffer, order_string);
        write(this->worker_pipes[j][WRITE], this->pipe_message_buffer, MSGSIZE);
    }
    OrderString *command = new OrderString(this->pipe_message_buffer);
    if (command == NULL)
    {
        printf("errno = % d \ n ", errno);
        perror("malloc");
    }
    orderType type = command->get_type();
    int deadline = -1;
    if (command->get_type() == search_type)
    {
        deadline = command->get_deadline();
    }

    this->receive_result(deadline);

    int counter = 0;
    // for (int i = 0; i < num_workers; i++)
    // {
    //     this->auditor[i]->print_results();
    // }
    if (command->get_type() == search_type)
    {
        alarm_in_use = false;
        for (int i = 0; i < num_workers; i++)
        {
            if (this->auditor[i]->alarm_answer_snapshot == true)
            {
                this->auditor[i]->print_results();
                counter++;
            }
        }
        printf("\nAnswerd : %d/%d\n", counter, this->num_workers);
    }
    else if (command->get_type() == mincount_type)
    {
        for (int i = 0; i < num_workers; i++)
        {
            this->auditor[i]->next_result->set_data();
        }
        int min = 99999;
        for (int i = 0; i < num_workers; i++)
        {
            if (min > this->auditor[i]->next_result->minimax && this->auditor[i]->next_result->minimax != -1)
            {
                min = this->auditor[i]->next_result->minimax;
            }
        }
        if (min == 99999)
        {
            printf("\nKeyword not found \n");
        }
        else
        {
            printf("\nThe min value is : %d \n", min);
        }
    }
    else if (command->get_type() == maxcount_type)
    {
        for (int i = 0; i < num_workers; i++)
        {
            this->auditor[i]->next_result->set_data();
        }
        int max = -1;
        for (int i = 0; i < num_workers; i++)
        {
            if (max < this->auditor[i]->next_result->minimax)
            {
                max = this->auditor[i]->next_result->minimax;
            }
        }
        if (max == -1)
        {
            printf("\nKeyword not found \n");
        }
        else
        {
            printf("\nThe max value is : %d \n", max);
        }
    }
    else if (command->get_type() == workercount_type)
    {
        unsigned long lines_count = 0;
        unsigned long words_count = 0;
        unsigned long bytes_count = 0;
        for (int i = 0; i < num_workers; i++)
        {
            this->auditor[i]->next_result->set_data();
            lines_count += this->auditor[i]->next_result->lines_count;
            words_count += this->auditor[i]->next_result->words_count;
            bytes_count += this->auditor[i]->next_result->bytes_count;
        }
        printf("\nThe total numbers are : lines - %lu,  words - %lu, bytes - %lu, \n", lines_count, words_count, bytes_count);
    }
    for (int i = 0; i < this->num_workers; i++)
    {
        this->auditor[i]->clean();
    }

    if (command->get_type() == exit_type)
    {
        delete (command);
        return 0;
    }
    else
    {
        delete (command);
        return 1;
    }
}

void WorkerHandler::do_action()
{
    bool found = false;
    char temp_buffer[BUFSIZE];
    char temp_buffer2[BUFSIZE];
    OrderString *command = new OrderString(this->pipe_message_buffer);
    if (command == NULL)
    {
        printf("errno = % d \ n ", errno);
        perror("malloc");
    }
    orderType type = command->get_type();

    switch (type)
    {
    case search_type: // /search ======================================================================================================================================
    {
        // if (a == 1)
        // {
        //     sleep(6);
        // }
        time_t ltime;
        time(&ltime);
        char *current_time = ctime(&ltime);
        current_time[strlen(current_time) - 1] = '\0';
        sprintf(temp_buffer2, "%s : %s : %s", current_time, get_enum_string(type), command->string);
        MapListNode *current_item = this->self_maps;
        while (current_item != NULL)
        {
            for (int i = 0; i < command->get_words_count(); i++)
            {
                char *word = command->get_word_by_index(i);
                PostingList *posting_list = current_item->map->get_words_posting_list(word);
                if (posting_list == NULL)
                {
                    continue;
                }
                PostingListNode *current_posting_list_node = posting_list->posting_list_next_node;

                while (current_posting_list_node != NULL)
                {
                    found = true;
                    int index = current_posting_list_node->get_index();
                    sprintf(temp_buffer, "/search %d %s %d %s", getpid(), current_item->map->filename, index, current_item->map->entries[index]->string);
                    strcpy(this->fifo_message_buffer, temp_buffer);
                    write(this->fifo_file_desc, this->fifo_message_buffer, MSGSIZE);
                    current_posting_list_node = current_posting_list_node->next;
                }
                if (strcmp(word, "-d") == 0)
                {
                    break;
                }
            }
            if (found == true)
            {
                sprintf(temp_buffer2, "%s : %s", temp_buffer2, current_item->map->filename);
                found = false;
            }

            current_item = current_item->next;
        }
        sprintf(temp_buffer2, "%s\n", temp_buffer2);
        write(this->worker_self_file_desc, temp_buffer2, strlen(temp_buffer2));
        //delete (word);
        break;
    }
    case maxcount_type: // /maxcount DONE =======================================================================================================================
    {
        int max = -1;
        MapListNode *current_item = this->self_maps;
        while (current_item != NULL)
        {
            int count = current_item->map->get_word_frequancy(command->get_string());
            if (count > max)
            {
                max = count;
            }
            current_item = current_item->next;
        }
        sprintf(temp_buffer, "/maxcount %d %d", getpid(), max);
        strcpy(this->fifo_message_buffer, temp_buffer);
        write(this->fifo_file_desc, this->fifo_message_buffer, MSGSIZE);
        break;
    }
    case mincount_type: // /mincount DONE =======================================================================================================================
    {
        int min = 999999;
        MapListNode *current_item = this->self_maps;
        while (current_item != NULL)
        {
            int count = current_item->map->get_word_frequancy(command->get_string());
            if (count < min)
            {
                min = count;
            }
            current_item = current_item->next;
        }
        sprintf(temp_buffer, "/mincount %d %d", getpid(), min);
        strcpy(this->fifo_message_buffer, temp_buffer);
        write(this->fifo_file_desc, this->fifo_message_buffer, MSGSIZE);
        break;
    }
    case workercount_type: // /wc DONE =========================================================================================================================
    {
        MapListNode *current_item = this->self_maps;
        unsigned long lines_count = 0;
        unsigned long words_count = 0;
        unsigned long bytes_count = 0;
        while (current_item != NULL)
        {
            lines_count += current_item->map->get_entry_count();
            words_count += current_item->map->get_words_count();
            bytes_count += current_item->map->get_bytes_count();
            current_item = current_item->next;
        }
        sprintf(temp_buffer, "/wc %d %lu %lu %lu", getpid(), lines_count, words_count, bytes_count);
        strcpy(this->fifo_message_buffer, temp_buffer);
        write(this->fifo_file_desc, this->fifo_message_buffer, MSGSIZE);
        break;
    }
    case path_distribution_type: // /path_distro DONE ============================================================================================================
    {
        this->self_maps = new MapListNode(command->get_string(), this->self_maps);
        if (this->self_maps == NULL)
        {
            printf("errno = % d \ n ", errno);
            perror("malloc");
        }
        break;
    }
    case exit_type: // /exit =====================================================================================================================================
    {
        delete (command);
        printf("\nExited succsessfully\n");
        close(this->worker_self_file_desc);
        sprintf(temp_buffer, "/end %d", getpid());
        strcpy(this->fifo_message_buffer, temp_buffer);
        write(this->fifo_file_desc, this->fifo_message_buffer, MSGSIZE);
        exit(EXIT_SUCCESS); // to remove data structs
    }
    case wrong_input: // wrond input =============================================================================================================================
    {
        printf("\nWrong Input - %s\n", this->pipe_message_buffer);
        break;
    }
    }
    sprintf(temp_buffer, "/end %d", getpid());
    strcpy(this->fifo_message_buffer, temp_buffer);
    write(this->fifo_file_desc, this->fifo_message_buffer, MSGSIZE);
    return;
}

void WorkerHandler::receive_result(int deadline)
{
    if (deadline != -1)
    {
        alarm_in_use = true;
        alarm(deadline);
        signal(SIGALRM, this->alarm_handler);
    }

    int temp_pid = 0;
    char temp_buf[BUFSIZE];
    char *temp = NULL;
    int rd;
    while (true)
    {
        if (rd = read(this->fifo_file_desc, this->fifo_message_buffer, MSGSIZE) < 0)
        {
            perror("problem in reading");
            exit(5);
        }
        if (rd == 0)
        {
            // printf("\nread returned 0\n");
            // sleep(1);
        }
        printf("Message Received: %s\n", this->fifo_message_buffer);
        char *result = new char[strlen(this->fifo_message_buffer) + 1];
        if (result == NULL)
        {
            printf("errno = % d \ n ", errno);
            perror("malloc");
        }
        strcpy(result, this->fifo_message_buffer);
        result[strlen(this->fifo_message_buffer)] = '\0';
        int addition = 0;
        orderType result_type;
        if ((temp = strstr(this->fifo_message_buffer, "/end")) != NULL)
        {
            temp_pid = atoi(temp + 5);
            for (int i = 0; i < this->num_workers; i++)
            {
                if (this->auditor[i]->get_pid() == temp_pid)
                {
                    this->auditor[i]->answered = true;
                }
            }
        }
        else
        {
            if ((temp = strstr(this->fifo_message_buffer, "/search")) != NULL)
            {
                result_type = search_type;
                addition = 8;
            }
            else if ((temp = strstr(this->fifo_message_buffer, "/mincount")) != NULL)
            {
                result_type = mincount_type;
                addition = 10;
            }
            else if ((temp = strstr(this->fifo_message_buffer, "/maxcount")) != NULL)
            {
                result_type = maxcount_type;
                addition = 10;
            }
            else if ((temp = strstr(this->fifo_message_buffer, "/wc")) != NULL)
            {
                result_type = workercount_type;
                addition = 3;
            }
            temp_pid = atoi(temp + addition);
            for (int i = 0; i < this->num_workers; i++)
            {
                if (this->auditor[i]->get_pid() == temp_pid)
                {
                    this->auditor[i]->add_result(result, result_type);
                }
            }
        }
        int count = 0;
        for (int i = 0; i < this->num_workers; i++)
        {
            if (this->auditor[i]->answered == true)
            {
                count++;
            }
        }
        if (count == this->num_workers)
        {
            for (int i = 0; i < this->num_workers; i++)
            {
                this->auditor[i]->answered = false;
            }
            return;
        }
    }
}

void WorkerHandler::initialize_worker(int i)
{
    printf("\nIam a child proccess %d\n", i);
    char temp_buff[20];
    sprintf(temp_buff, "./log/Worker_%d", getpid());
    this->worker_self_file_desc = open(temp_buff, O_CREAT | O_WRONLY | O_APPEND);

    if ((this->fifo_file_desc = open(NAMED_PIPE_FILE_NAME, O_WRONLY)) < 0)
    {
        perror("fifo open error");
        exit(1);
    }

    if (i == -1)
    {
        for (int i = 0; i < num_workers; i++)
        {
            if (this->auditor[i]->get_pid() == killed_pid)
            {
                PathListNode *temp = this->auditor[i]->next_path;
                while (temp != NULL)
                {
                    this->self_maps = new MapListNode(temp->string, this->self_maps);
                    if (this->self_maps == NULL)
                    {
                        printf("errno = % d \ n ", errno);
                        perror("malloc");
                    }
                    temp = temp->next;
                }
            }
        }
    }

    while (1)
    {
        for (int j = 0; j < this->num_workers; j++)
        {
            close(this->worker_pipes[i][WRITE]);
        }
        read(this->worker_pipes[i][READ], this->pipe_message_buffer, MSGSIZE);

        this->do_action();
    }
}

char *get_enum_string(orderType type)
{
    switch (type)
    {
    case search_type:
        return "/search";
    case maxcount_type:
        return "/maxcount";
    case mincount_type:
        return "/mincount";
    case workercount_type:
        return "/wc";
    case exit_type:
        return "/exit";
    }
}

void WorkerHandler::alarm_handler(int signo)
{
    if (alarm_in_use)
    {
        for (int i = 0; i < global_num_wrokers; i++)
        {
            auditor_snapshot[i]->alarm_answer_snapshot = auditor_snapshot[i]->answered;
        }
        printf("\nAllarmed!\n");
    }
}

void WorkerHandler::signal_handler(int signo)
{
    if (signal_in_use)
    {
        pid_t pid;
        pid = wait(NULL);
        killed_pid = pid;
    }
}
