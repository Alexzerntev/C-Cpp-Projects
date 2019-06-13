#include "./error_handler.h"

void print_error(int error_code, const char *message)
{
    if (error_code == MEMORY_ALLOCATION_ERROR)
    {
        printf("%s\n", message);
        printf("Failed to allocate memory\n");
        perror("malloc");
        return;
    }
    else if (error_code == FLOAT_CONVERTION_ERROR)
    {
        printf("%s\n", message);
        printf("Failed to convert string to float\n");
        perror("atof");
        return;
    }
    else if (error_code == INT_CONVERTION_ERROR)
    {
        printf("%s\n", message);
        printf("Failed to convert string to int\n");
        perror("atoi");
        return;
    }
    else if (error_code == WRONG_INITIALIZATION_ERROR)
    {
        printf("%s\n", message);
        printf("Failed to initialize the app\n");
        return;
    }
    else if (error_code == STRING_PARSING_ERROR)
    {
        printf("%s\n", message);
        printf("Failed to parse string\n");
        return;
    }
    else if (error_code == WRONG_INPUT)
    {
        printf("%s\n", message);
        printf("Wrong Input\n");
        return;
    }
    else if (error_code == DIRECTORY_DOES_NOT_EXIST)
    {
        printf("%s\n", message);
        printf("Directory does not exist\n");
        return;
    }
    else if (error_code == COULD_NOT_CREATE_DIRECTORY)
    {
        printf("%s\n", message);
        printf("Could not create directory\n");
        return;
    }
    else if (error_code == DIRECTORY_ERROR)
    {
        printf("%s\n", message);
        printf("Something went wrong in the directory\n");
        return;
    }
    else if (error_code == FILE_OPENING_ERROR)
    {
        printf("%s\n", message);
        printf("Could not open file\n");
        return;
    }
    else if (error_code == FILE_WRITING_ERROR)
    {
        printf("%s\n", message);
        printf("Could not write to a file\n");
        return;
    }
    else if (error_code == FILE_CLOSING_ERROR)
    {
        printf("%s\n", message);
        printf("Could not close file\n");
        return;
    }
    else if (error_code == WRITE_TO_FILE_ERROR)
    {
        printf("%s\n", message);
        printf("Could not write to file\n");
        return;
    }
    else if (error_code == COULD_NOT_CREATE_FIFO)
    {
        printf("%s\n", message);
        printf("Could not create fifo\n");
        return;
    }
    else if (error_code == COULD_NOT_OPEN_FIFO)
    {
        printf("%s\n", message);
        printf("Could not open fifo\n");
        return;
    }
    else if (error_code == COULD_NOT_OPEN_FILE)
    {
        printf("%s\n", message);
        printf("Could not open file\n");
        return;
    }
    else if (error_code == COULD_NOT_WRITE_TO_FIFO)
    {
        printf("%s\n", message);
        printf("Could not write to fifo\n");
        return;
    }
    else if (error_code == COULD_NOT_READ_FROM_FIFO)
    {
        printf("%s\n", message);
        printf("Could not write to fifo\n");
        return;
    }
    else if (error_code == COULD_NOT_WRITE_TO_FILE)
    {
        printf("%s\n", message);
        printf("Could not write to file\n");
        return;
    }
    else if (error_code == COULD_NOT_CLOSE_FILE)
    {
        printf("%s\n", message);
        printf("Could not close file\n");
        return;
    }
    else if (error_code == COULD_NOT_CLOSE_FIFO)
    {
        printf("%s\n", message);
        printf("Could not close fifo\n");
        return;
    }
    else if (error_code == COULD_NOT_LOCK_FILE)
    {
        printf("%s\n", message);
        printf("Could not lock file\n");
        return;
    }
    else if (error_code == COULD_NOT_UNLOCK_FILE)
    {
        printf("%s\n", message);
        printf("Could not unlock file\n");
        return;
    }
    else if (error_code == NOT_FOUND)
    {
        printf("%s\n", message);
        printf("Entry not found\n");
        return;
    }
    else if (error_code == SOCKET_FAILED)
    {
        printf("%s\n", message);
        perror("socket failed");
        printf("Failed to initialize socket\n");
        return;
    }
    else if (error_code == SOCKET_BIND_ERROR)
    {
        printf("%s\n", message);
        perror("bind");
        printf("Failed to bind socket\n");
        return;
    }
    else if (error_code == SOCKET_LISTEN_ERROR)
    {
        printf("%s\n", message);
        perror("listen");
        printf("Failed to listen on socket\n");
        return;
    }
    else if (error_code == SOCKET_ACCEPT_ERROR)
    {
        printf("%s\n", message);
        perror("accept");
        printf("Failed to accept in socket\n");
        return;
    }
    else if (error_code == GET_HOST_BY_ADDRESS_ERROR)
    {
        printf("%s\n", message);
        perror("gethostbyadr");
        printf("Failed to get host by address \n");
        return;
    }
    else if (error_code == BUFFER_FULL)
    {
        printf("%s\n", message);
        printf("The buffer is full\n");
        return;
    }
    else if (error_code == BUFFER_EMPTY)
    {
        printf("%s\n", message);
        printf("The buffer is empty\n");
        return;
    }
    else
    {
        printf("%s\n", message);
    }
}