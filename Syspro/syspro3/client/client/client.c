#include "./client.h"

CycBuffer *cyc_buffer = NULL;
ConListNode *con_list = NULL;
int exit_requested = 0;

pthread_mutex_t lock;
pthread_cond_t empty;
pthread_cond_t full;

extern ClientSettings *global_settings;

int sock;                  /* The socket file descriptor for our "listening"
                   	socket */
int connectlist[MAX_CONS]; /* Array of connected sockets so we know who
	 		we are talking to */
fd_set socks;              /* Socket file descriptors we want to wake
			up for, using select() */
int highsock;

int connect_to_server(ClientSettings *settings)
{
    int res;

    res = pthread_mutex_init(&lock, NULL);
    if (res != 0)
    {
        printf("\n mutex init failed\n");
        return -1;
    }
    res = pthread_cond_init(&empty, NULL);
    if (res != 0)
    {
        printf("\n mutex init failed\n");
        return -1;
    }

    res = pthread_cond_init(&full, NULL);
    if (res != 0)
    {
        printf("\n mutex init failed\n");
        return -1;
    }

    int sockfd;
    struct sockaddr_in servaddr;

    // socket create and varification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
    {
        printf("Client Socket successfully created..\n");
    }
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(settings->server_ip);
    servaddr.sin_port = htons(settings->server_port);

    // connect the client socket to server socket
    if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0)
    {
        perror("connect");
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
    {
        printf("connected to the server..\n");
    }

    return sockfd;
}

void send_log_on(int sockfd)
{
    char buf[1024] = {0x0};
    char ok[] = "--";

    struct ifaddrs *ifap, *ifa;
    struct sockaddr_in *sa;

    getifaddrs(&ifap);
    for (ifa = ifap; ifa; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr->sa_family == AF_INET)
        {
            sa = (struct sockaddr_in *)ifa->ifa_addr;
        }
    }

    freeifaddrs(ifap);

    sprintf(buf, "LOG_ON<%u,%d>", htonl((unsigned int)(sa->sin_addr.s_addr)), global_settings->port_num);
    if (write(sockfd, buf, strlen(buf) + 1) < 0)
    {
        perror("write");
        exit(1);
    }

    if (read(sockfd, ok, strlen(ok)) < 0)
    {
        perror("read");
        exit(1);
    }
}

int connect_to_client(BufferItem *buffer_item)
{
    int sockfd;
    struct sockaddr_in servaddr;

    // socket create and varification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
    {
        printf("Client Socket successfully created..\n");
    }
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = buffer_item->ip;
    servaddr.sin_port = htons(buffer_item->port);

    // connect the client socket to server socket
    while (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0)
    {
        // perror("connect");
        // printf("connection with the server failed...\n");
        // exit(0);
    }
    printf("connected to client...\n");

    return sockfd;
}

void get_clients_from_server(int server_socket)
{
    char buf[1024] = {0x0};
    char one_buf[1] = {0x0};
    int count = 0;

    sprintf(buf, "%s", "GET_CLIENTS");
    printf("Sendind %s\n", buf);
    if (write(server_socket, buf, strlen(buf) + 1) < 0)
    {
        perror("write");
        exit(1);
    }

    count = 0;
    while (read(server_socket, one_buf, 1) > 0)
    {
        buf[count] = one_buf[0];
        count++;

        if (one_buf[0] == '\0')
        {
            break;
        }
    }

    printf("Received:%s\n", buf);

    int index = index_of(buf, ' ');
    char *ptr = buf + index + 1;
    int times = atoi(ptr);
    int lt_count = 0;

    count = 0;
    memset(buf, 0, 1024);

    if (times != 0)
    {
        while (read(server_socket, one_buf, 1) > 0)
        {
            buf[count] = one_buf[0];
            count++;
            if (one_buf[0] == '>')
            {
                printf("Received: %s \n", buf);
                buf[count] = '\0';
                BufferItem *item = to_buffer_item(buf, 0, -1, server_socket, 0);
                BufferItem *item2 = to_buffer_item(buf, 0, -1, server_socket, 0);
                write_to_buffer(item);
                add_to_con_list(&con_list, item2);
                lt_count++;
                count = 0;
            }
            if (lt_count == times)
            {
                break;
            }
        }
    }
    // buf[0] = '#';
    // if (write(server_socket, buf, strlen(buf)) < 0)
    // {
    //     perror("write");
    //     exit(1);
    // }
    printf("Closing connection\n");
    close(server_socket);
    printf("The initial communication with sever terminated successfully\n");
}

void disconnect_from_server(int server_socket)
{
    char buf[1024] = {0x0};

    struct ifaddrs *ifap, *ifa;
    struct sockaddr_in *sa;

    getifaddrs(&ifap);
    for (ifa = ifap; ifa; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr->sa_family == AF_INET)
        {
            sa = (struct sockaddr_in *)ifa->ifa_addr;
        }
    }

    freeifaddrs(ifap);

    sprintf(buf, "LOG_OFF<%u,%d>", htonl((unsigned int)(sa->sin_addr.s_addr)), global_settings->port_num);
    printf("Sengind %s\n", buf);
    if (write(server_socket, buf, strlen(buf) + 1) < 0)
    {
        perror("write");
        exit(1);
    }
}
// ===================================================================================================
//                      Here starts the responding part
// ===================================================================================================
void welcoming(ClientSettings *settings)
{
    int port;                          /* The port number after conversion from ascport */
    struct sockaddr_in server_address; /* bind info structure */
    int reuse_addr = 1;
    struct timeval timeout; /* Timeout for select */
    int readsocks;          /* Number of sockets ready for reading */

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse_addr,
               sizeof(reuse_addr));

    setnonblocking(sock);

    port = settings->port_num; /* Use function from sockhelp to
                                    convert to an int */
    memset((char *)&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(port);
    if (bind(sock, (struct sockaddr *)&server_address,
             sizeof(server_address)) < 0)
    {
        perror("bind");
        close(sock);
        exit(EXIT_FAILURE);
    }

    listen(sock, 5);

    highsock = sock;
    memset((char *)&connectlist, 0, sizeof(connectlist));

    while (1)
    { /* Main server loop - forever */
        build_select_list();
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        readsocks = select(highsock + 1, &socks, (fd_set *)0,
                           (fd_set *)0, &timeout);

        if (readsocks < 0)
        {
            perror("select");
            exit(EXIT_FAILURE);
        }
        if (readsocks == 0)
        {
            /* Nothing ready to read, just show that
			   we're alive */
            // printf(".");
            fflush(stdout);
        }
        else
        {
            read_socks(settings);
        }
    } /* while(1) */
}

void setnonblocking(int sock)
{
    int opts;

    opts = fcntl(sock, F_GETFL);
    if (opts < 0)
    {
        perror("fcntl(F_GETFL)");
        exit(EXIT_FAILURE);
    }
    opts = (opts | O_NONBLOCK);
    if (fcntl(sock, F_SETFL, opts) < 0)
    {
        perror("fcntl(F_SETFL)");
        exit(EXIT_FAILURE);
    }
    return;
}

void build_select_list()
{
    int listnum; /* Current item in connectlist for for loops */

    FD_ZERO(&socks);

    FD_SET(sock, &socks);

    for (listnum = 0; listnum < MAX_CONS; listnum++)
    {
        if (connectlist[listnum] != 0)
        {
            FD_SET(connectlist[listnum], &socks);
            if (connectlist[listnum] > highsock)
                highsock = connectlist[listnum];
        }
    }
}

void handle_new_connection()
{
    int listnum;    /* Current item in connectlist for for loops */
    int connection; /* Socket file descriptor for incoming connections */

    struct sockaddr_in client;
    socklen_t clientlen;
    struct sockaddr *clientptr = (struct sockaddr *)&client;

    connection = accept(sock, clientptr, &clientlen);
    if (exists_ip(con_list, (unsigned int)client.sin_addr.s_addr) == 0 &&
        !((unsigned int)client.sin_addr.s_addr == (unsigned int)inet_addr(global_settings->server_ip)))
    {
        printf("The connected client is not known, closing connection...");
        close(connection);
        return;
    }

    setnonblocking(connection);
    for (listnum = 0; (listnum < MAX_CONS) && (connection != -1); listnum++)
        if (connectlist[listnum] == 0)
        {
            printf("\nConnection accepted:   FD=%d; Slot=%d\n",
                   connection, listnum);
            connectlist[listnum] = connection;
            connection = -1;
        }
    if (connection != -1)
    {
        printf("\nNo room left for new client.\n");
        close(connection);
    }
}

void read_socks(ClientSettings *clinet_settings)
{
    int listnum; /* Current item in connectlist for for loops */

    if (FD_ISSET(sock, &socks))
    {
        handle_new_connection();
    }

    for (listnum = 0; listnum < MAX_CONS; listnum++)
    {
        if (FD_ISSET(connectlist[listnum], &socks))
        {
            deal_with_data(listnum, clinet_settings);
        }
    } /* for (all entries in queue) */
}

void deal_with_data(int listnum, ClientSettings *client_settings)
{
    char buf[1024];
    int count = 0;
    int read_dt = 0;
    while (1)
    {
        count = 0;
        while ((read_dt = read(connectlist[listnum], buf + count, 1)) > 0)
        { /* Receive 1 char */
            count++;
        }

        if (read_dt < 0 && count == 0)
        {
            printf("\nConnection terminated: FD=%d;  Slot=%d\n", connectlist[listnum], listnum);
            close(connectlist[listnum]);
            connectlist[listnum] = 0;
            return;
        }

        if (count == 0)
        {
            return;
        }

        buf[count] = '\0';
        printf("Received %s\n", buf);

        char *ptr;
        if ((ptr = strstr(buf, "GET_FILE_LIST")) != NULL)
        {
            give_file_list(listnum, client_settings);
        }
        if ((ptr = strstr(buf, "GET_FILE")) != NULL && strstr(buf, "GET_FILE_LIST") == NULL)
        {
            BufferItem *buffer_item = to_buffer_item(ptr, 1, listnum, -1, 0);

            give_file(listnum, buffer_item, client_settings);
            destroy_cyc_buffer_item(buffer_item, 1);
            return;
        }

        if ((ptr = strstr(buf, "USER_OFF")) != NULL)
        {
            BufferItem *buffer_item = to_buffer_item(buf, 0, -1, -1, 0);
            remove_by_ip_port(&con_list, buffer_item->ip, buffer_item->port);
            mark_left_buffer(cyc_buffer, buffer_item->ip, buffer_item->port);
            destroy_cyc_buffer_item(buffer_item, 1);
            printf("\nConnection terminated: FD=%d;  Slot=%d\n", connectlist[listnum], listnum);
            close(connectlist[listnum]);
            connectlist[listnum] = 0;
            return;
        }

        if ((ptr = strstr(buf, "USER_ON")) != NULL)
        {
            BufferItem *buffer_item = to_buffer_item(buf, 0, listnum, -1, 0);
            BufferItem *buffer_item2 = to_buffer_item(buf, 0, listnum, -1, 0);
            add_to_con_list(&con_list, buffer_item);
            write_to_buffer(buffer_item2);
            printf("\nConnection terminated: FD=%d;  Slot=%d\n", connectlist[listnum], listnum);
            close(connectlist[listnum]);
            connectlist[listnum] = 0;
            return;
        }
    }
}

void give_file_list(int listnum, ClientSettings *settings)
{
    char local_buffer[512];
    char file_name_buffer[512];
    char buf[512];
    char sum_buf[512];
    FILE *fp;
    size_t length;
    char *line = NULL;
    int files_total = 0;

    // Getting the sum of files
    sprintf(sum_buf, "%s/file_list", settings->dir_name);
    files_total += get_line_count(sum_buf);

    sprintf(sum_buf, "FILE_LIST %d", files_total);
    printf("Writing %s\n", sum_buf);
    if (write(connectlist[listnum], sum_buf, strlen(sum_buf) + 1) < 0)
    {
        perror("writing FILE_LIST");
        exit(1);
    }

    // send files
    sprintf(local_buffer, "%s/file_list", settings->dir_name);

    printf("====== Opening file %s\n", local_buffer);

    fp = fopen(local_buffer, "r");

    if (fp == NULL)
    {
        print_error(READ_FILE_ERROR, "List opening\n");
        close(connectlist[listnum]);
        kill(getppid(), SIGUSR2);
        exit(1);
    }
    while (getline(&line, &length, fp) != EOF)
    {
        // sending names
        remove_linebreak(line);
        printf("====== Sending name : %s\n", line);

        sprintf(file_name_buffer, "%s/%s", settings->dir_name, line);
        char *hash = {0x0};
        hash = get_hash_of_file(file_name_buffer);
        sprintf(buf, "<%s,%s>", line, hash);
        destroy_string(hash);
        if (write(connectlist[listnum], buf, strlen(buf)) < 0)
        {
            perror("writing dir name");
            exit(1);
        }
    }

    fclose(fp);
    if (line != NULL)
    {
        free(line);
    }
    line = NULL;
}

void give_file(int listnum, BufferItem *buffer_item, ClientSettings *settings)
{
    char full_name[1024];
    char buff[1024];
    int result_code = SUCCESS;

    sprintf(full_name, "%s/%s", settings->dir_name, buffer_item->path);
    printf("Sending file: %s \n", full_name);
    if (file_exists(full_name) == 0)
    {
        sprintf(buff, "FILE_NOT_FOUND");
        if (write(connectlist[listnum], buff, strlen(buff) + 1) < 0)
        {
            perror("writing FILE_NOT_FOUND");
            exit(1);
        }
        return;
    }

    if (check_if_same_version(full_name, buffer_item->version) == 1)
    {
        sprintf(buff, "FILE_UP_TO_DATE");
        if (write(connectlist[listnum], buff, strlen(buff)) < 0)
        {
            perror("writing FILE_UP_TO_DATE");
            exit(1);
        }
        return;
    }

    int file_size = get_file_size(full_name, &result_code);
    sprintf(buff, "FILE_SIZE %d", file_size);
    if (write(connectlist[listnum], buff, strlen(buff) + 1) < 0)
    {
        perror("writing FILE_NOT_FOUND");
        exit(1);
    }

    write_file_in_chunks(full_name, file_size, connectlist[listnum], &result_code);
}

// ===================================================================================================
//                      Here starts the requesting part
// ===================================================================================================

int write_file_in_chunks(char *filename, int length, int fifo_fd, int *result_code)
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
        num_bytes = read(fd, local_buffer, 400);
        total_count += num_bytes;
        printf("====== Writing file : %s\n", filename);

        if (write(fifo_fd, local_buffer, num_bytes) != num_bytes)
        {
            print_error(COULD_NOT_WRITE_TO_FIFO, "Writer to fifo");
            return COULD_NOT_WRITE_TO_FIFO;
        }
        if (num_bytes == 0 || num_bytes < 400)
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

int receive_file(char *file_name, char *fifo_buffer, int buffer_size, unsigned int file_size, int fifo_fd, int *result_code)
{
    int bytes_read, bytes_wrote, total_bytes = 0, bytes_read_local = 0;
    unsigned int count = file_size;

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

void write_to_buffer(BufferItem *buffer_item)
{
    if (exit_requested == 1)
    {
        pthread_mutex_unlock(&lock);
        pthread_cond_broadcast(&full);
        pthread_cond_broadcast(&empty);
        pthread_exit(0);
    }
    pthread_mutex_lock(&lock);
    while (is_cyc_buffer_full(cyc_buffer) == 1)
    {
        printf("The buffer is full, going to wait...\n");
        pthread_cond_wait(&full, &lock);
        printf("A thread woke up\n");
        if (exit_requested == 1)
        {
            pthread_mutex_unlock(&lock);
            pthread_cond_broadcast(&full);
            pthread_cond_broadcast(&empty);
            pthread_exit(0);
        }
    }
    add_to_buffer(cyc_buffer, buffer_item);
    pthread_mutex_unlock(&lock);
    pthread_cond_broadcast(&empty);
}

void *read_from_buffer(void *arg)
{
    while (1)
    {
        if (exit_requested == 1)
        {
            pthread_mutex_unlock(&lock);
            pthread_cond_broadcast(&full);
            pthread_cond_broadcast(&empty);
            pthread_exit(0);
        }

        pthread_mutex_lock(&lock);
        while (is_cyc_buffer_empty(cyc_buffer) == 1)
        {
            printf("The buffer is empty, going to wait...\n");
            pthread_cond_wait(&empty, &lock);
            printf("A thread woke up\n");
            if (exit_requested == 1)
            {
                pthread_mutex_unlock(&lock);
                pthread_cond_broadcast(&full);
                pthread_cond_broadcast(&empty);
                pthread_exit(0);
            }
        }

        BufferItem *bufer_item = remove_from_buffer(cyc_buffer);
        pthread_mutex_unlock(&lock);
        pthread_cond_broadcast(&full);
        if (bufer_item != NULL)
        {
            deal_with_buffer_item(bufer_item);
        }
    }
}

void deal_with_buffer_item(BufferItem *buffer_item)
{
    if (buffer_item->left == 1)
    {
        return;
    }

    int fd = connect_to_client(buffer_item);
    char buf[1024] = {0x0};
    char new_file_buf[200] = {0x0};
    char one_buf[1] = {0x0};
    int result_code = SUCCESS;
    int count = 0;
    if (buffer_item->path == NULL)
    {
        sprintf(new_file_buf, "remote_files/%d_%d_%d/", global_settings->port_num, buffer_item->ip, buffer_item->port);
        if (dir_exists(new_file_buf) == 0)
        {
            create_custom_dir(new_file_buf);
        }

        sprintf(buf, "%s", "GET_FILE_LIST");
        printf("Sendind %s\n", buf);
        if (write(fd, buf, strlen(buf) + 1) < 0)
        {
            perror("write");
            exit(1);
        }

        count = 0;
        while (read(fd, one_buf, 1) > 0)
        {
            buf[count] = one_buf[0];
            count++;

            if (one_buf[0] == '\0')
            {
                break;
            }
        }

        printf("Received:%s\n", buf);

        int index = index_of(buf, ' ');
        char *ptr = buf + index + 1;
        int times = atoi(ptr);
        int lt_count = 0;

        count = 0;
        memset(buf, 0, 1024);

        if (times != 0)
        {
            while (read(fd, one_buf, 1) > 0)
            {
                buf[count] = one_buf[0];
                count++;
                if (one_buf[0] == '>')
                {
                    printf("Received: %s \n", buf);
                    BufferItem *item = to_buffer_item(buf, 1, -1, fd, 0);
                    item->ip = buffer_item->ip;
                    item->port = buffer_item->port;
                    write_to_buffer(item);
                    if (result_code != SUCCESS)
                    {
                        print_error(result_code, "In reading client list");
                    }
                    lt_count++;
                    count = 0;
                }
                if (lt_count == times)
                {
                    break;
                }
            }
        }
        printf("The receiving file list terminated succesfully\n");
    }
    else
    {
        char buff[300];
        char bufff[1024];

        sprintf(buff, "./remote_files/%d_%d_%d/%s", global_settings->port_num, buffer_item->ip, buffer_item->port, buffer_item->path);
        create_recursive_dir(buff);
        int same = check_if_same_version(buff, buffer_item->version);
        // no need to get it, its the same
        if (same == 1)
        {
            printf("The file is up to date, returning...\n");
            return;
        }
        else
        {
            if (buffer_item->version != NULL)
            {
                destroy_string(buffer_item->version);
            }

            buffer_item->version = copy_const_string("*", &result_code);
        }

        sprintf(buf, "GET_FILE<%s,%s>", buffer_item->path, buffer_item->version);
        printf("Sendind %s\n", buf);
        if (write(fd, buf, strlen(buf) + 1) < 0)
        {
            perror("write");
            exit(1);
        }

        count = 0;
        while (read(fd, one_buf, 1) > 0)
        {
            buf[count] = one_buf[0];
            count++;

            if (one_buf[0] == '\0')
            {
                break;
            }
        }

        printf("Received:%s\n", buf);

        char *ptr1;
        if ((ptr1 = strstr(buf, "FILE_NOT_FOUND")) != NULL)
        {
            printf("For file %s\n", buffer_item->path);
            return;
        }

        if ((ptr1 = strstr(buf, "FILE_UP_TO_DATE")) != NULL)
        {
            printf("For file %s\n", buffer_item->path);
            return;
        }

        int index = index_of(buf, ' ');
        char *ptr = buf + index + 1;
        int times = atoi(ptr);

        receive_file(buff, bufff, 1024, times, fd, &result_code);

        printf("The receiving file list terminated succesfully\n");
        close(fd);
    }
}