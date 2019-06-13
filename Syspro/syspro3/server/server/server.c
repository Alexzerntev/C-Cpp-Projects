#include "./server.h"

ConListNode *con_list = NULL;

pthread_mutex_t mutex;
int sock;                  /* The socket file descriptor for our "listening"
                   	socket */
int connectlist[MAX_CONS]; /* Array of connected sockets so we know who
	 		we are talking to */
fd_set socks;              /* Socket file descriptors we want to wake
			up for, using select() */
int highsock;              /* Highest #'d file descriptor, needed for select() */

void initialize_and_operate_server(ServerSettings *settings)
{
    int port;                          /* The port number after conversion from ascport */
    struct sockaddr_in server_address; /* bind info structure */
    int reuse_addr = 1;                /* Used so we can re-bind to our port
				while a previous connection is still
				in TIME_WAIT state. */
    struct timeval timeout;            /* Timeout for select */
    int readsocks;                     /* Number of sockets ready for reading */

    /* Obtain a file descriptor for our "listening" socket */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    /* So that we can re-bind to it without TIME_WAIT problems */
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse_addr,
               sizeof(reuse_addr));

    /* Set socket to non-blocking with our setnonblocking routine */
    setnonblocking(sock);

    if (pthread_mutex_init(&mutex, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return;
    }

    /* Get the address information, and bind it to the socket */
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

    /* Set up queue for incoming connections. */
    listen(sock, 5);

    /* Since we start with only one socket, the listening socket,
	   it is the highest socket so far. */
    highsock = sock;
    memset((char *)&connectlist, 0, sizeof(connectlist));

    while (1)
    { /* Main server loop - forever */
        build_select_list();
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        /* The first argument to select is the highest file
			descriptor value plus 1. In most cases, you can
			just pass FD_SETSIZE and you'll be fine. */

        /* The second argument to select() is the address of
			the fd_set that contains sockets we're waiting
			to be readable (including the listening socket). */

        /* The third parameter is an fd_set that you want to
			know if you can write on -- this example doesn't
			use it, so it passes 0, or NULL. The fourth parameter
			is sockets you're waiting for out-of-band data for,
			which usually, you're not. */

        /* The last parameter to select() is a time-out of how
			long select() should block. If you want to wait forever
			until something happens on a socket, you'll probably
			want to pass NULL. */

        readsocks = select(highsock + 1, &socks, (fd_set *)0,
                           (fd_set *)0, &timeout);

        /* select() returns the number of sockets that had
			things going on with them -- i.e. they're readable. */

        /* Once select() returns, the original fd_set has been
			modified so it now reflects the state of why select()
			woke up. i.e. If file descriptor 4 was originally in
			the fd_set, and then it became readable, the fd_set
			contains file descriptor 4 in it. */

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
            read_socks();
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

    /* First put together fd_set for select(), which will
	   consist of the sock veriable in case a new connection
	   is coming in, plus all the sockets we have already
	   accepted. */

    /* FD_ZERO() clears out the fd_set called socks, so that
		it doesn't contain any file descriptors. */

    FD_ZERO(&socks);

    /* FD_SET() adds the file descriptor "sock" to the fd_set,
		so that select() will return if a connection comes in
		on that socket (which means you have to do accept(), etc. */

    FD_SET(sock, &socks);

    /* Loops through all the possible connections and adds
		those sockets to the fd_set */

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

    /* We have a new connection coming in!  We'll
	try to find a spot for it in connectlist. */
    connection = accept(sock, NULL, NULL);
    if (connection < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
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
        /* No room left in the queue! */
        printf("\nNo room left for new client.\n");
        // sock_puts(connection, "Sorry, this server is too busy.   Try again later!\r\n");
        close(connection);
    }
}

void read_socks()
{
    int listnum; /* Current item in connectlist for for loops */

    /* OK, now socks will be set with whatever socket(s)
	   are ready for reading.  Lets first check our
	   "listening" socket, and then check the sockets
	   in connectlist. */

    /* If a client is trying to connect() to our listening
		socket, select() will consider that as the socket
		being 'readable'. Thus, if the listening socket is
		part of the fd_set, we need to accept a new connection. */

    if (FD_ISSET(sock, &socks))
    {
        handle_new_connection();
    }
    /* Now check connectlist for available data */

    /* Run through our sockets and check to see if anything
		happened with them, if so 'service' them. */

    for (listnum = 0; listnum < MAX_CONS; listnum++)
    {
        if (FD_ISSET(connectlist[listnum], &socks))
        {
            deal_with_data(listnum);
        }
    } /* for (all entries in queue) */
}

void deal_with_data(
    int listnum /* Current item in connectlist for for loops */
)
{
    char ok[] = "ok";
    char buf[1024];
    char *ptr;
    int count = 0;
    int read_dt = 0;
    while (1)
    {
        count = 0;
        while ((read_dt = read(connectlist[listnum], buf + count, 1)) > 0)
        { /* Receive 1 char */
            count++;
        }

        // if (buf[0] == '#')
        // {
        //     // close(connectlist[listnum]);
        //     // connectlist[listnum] = 0;
        //     return;
        // }

        if (read_dt < 0 && count == 0)
        {
            return;
        }

        if (count == 0)
        {
            return;
        }

        printf("Received %s\n", buf);

        if ((ptr = strstr(buf, "LOG_ON")) != NULL)
        {
            printf("Informing clients about new client \n");
            inform_on_clients(buf, listnum);
            if (write(connectlist[listnum], ok, strlen(ok)) < 0)
            {
                printf("Error in writing \"ok\"\n");
            }
        }

        if ((ptr = strstr(buf, "LOG_OFF")) != NULL)
        {
            printf("Informing clients about client leaving \n");
            BufferItem *temp = to_buffer_item(buf, 0, -1, -1, 0);
            inform_off_clients(temp);
            destroy_cyc_buffer_item(temp,1);
            return;
        }

        if ((ptr = strstr(buf, "GET_CLIENTS")) != NULL)
        {
            printf("Serving existing clients\n");
            serve_clients(listnum);
            printf("Returning...\n");
        }
    }
}

// User on request handling
void inform_on_clients(char *string, int list_num)
{
    int result_code = SUCCESS;
    BufferItem *buffer_item = to_buffer_item(string, 0, list_num, -1, 0);
    ConListNode *tmp = con_list;

    while (tmp != NULL)
    {
        printf("Sending to client with ip num: %d\n", tmp->bufer_item->ip);
        send_user_on(buffer_item, tmp->bufer_item);
        printf("Sent successfully\n");
        tmp = tmp->next;
    }

    printf("Adding to list of connected clients\n");
    pthread_mutex_lock(&mutex);
    result_code = add_to_con_list(&con_list, buffer_item);
    pthread_mutex_unlock(&mutex);
    printf("Added succesfully\n");
    if (result_code != SUCCESS)
    {
        print_error(result_code, "In server adding to list");
    }
}

void send_user_on(BufferItem *node, BufferItem *client_to_inform)
{
    char bf[1024] = {0x0};
    char bff[512] = {0x0};
    char command[] = "USER_ON";
    int fd = connect_to_client(client_to_inform);

    from_buffer_item(node, bff);
    sprintf(bf, "%s%s", command, bff);

    if (write(fd, bf, strlen(bf) + 1) < 0)
    {
        print_error(COULD_NOT_WRITE_TO_FIFO, "USER_ON socket writing");
        perror("write");
    }
    printf("Informed client about USER_ON\n");
}

// User off request handling
void inform_off_clients(BufferItem *buffer_item)
{
    ConListNode *removed = get_remove_by_port_ip(&con_list, buffer_item);

    close(connectlist[removed->bufer_item->list_num]);
    connectlist[removed->bufer_item->list_num] = 0;
    ConListNode *temp = con_list;

    while (temp != NULL)
    {
        send_user_off(removed->bufer_item, temp->bufer_item);
        temp = temp->next;
    }

    destroy_con_node(removed);
}

void send_user_off(BufferItem *node, BufferItem *client_to_inform)
{
    char bf[1024] = {0x0};
    char bff[512] = {0x0};
    char command[] = "USER_OFF";
    int fd = connect_to_client(client_to_inform);

    from_buffer_item(node, bff);
    sprintf(bf, "%s%s", command, bff);

    if (write(fd, bf, strlen(bf) + 1) < 0)
    {
        print_error(COULD_NOT_WRITE_TO_FIFO, "USER_OFF socket writing");
        perror("write");
    }
    printf("Informed client about USER_OFF\n");
    close(fd);
}

// Get clients request handling
void serve_clients(int list_num)
{
    ConListNode *tmp = con_list;
    char command[100];
    sprintf(command, "CLIENT_LIST %d ", get_con_list_size(con_list) - 1);
    printf("Sending: %s\n", command);
    if (write(connectlist[list_num], command, strlen(command) + 1) < 0)
    {
        print_error(COULD_NOT_WRITE_TO_FIFO, "CLIENT_LIST socket writing");
        perror("write");
    }

    while (tmp != NULL)
    {
        if (list_num != tmp->bufer_item->list_num)
        {
            send_client(tmp->bufer_item, list_num);
        }
        tmp = tmp->next;
    }
}

void send_client(BufferItem *node, int list_num)
{
    char bf[1024] = {0x0};
    from_buffer_item(node, bf);
    printf("Sending client: %s\n", bf);
    if (write(connectlist[list_num], bf, strlen(bf) + 1) < 0)
    {
        print_error(COULD_NOT_WRITE_TO_FIFO, "USER_ON socket writing");
        perror("write");
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
    if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0)
    {
        perror("connect");
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
    {
        printf("connected to client...\n");
    }

    return sockfd;
}
