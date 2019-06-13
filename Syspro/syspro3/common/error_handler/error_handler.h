#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#define SUCCESS 0
#define MEMORY_ALLOCATION_ERROR 1
#define WRONG_INITIALIZATION_ERROR 2
#define FLOAT_CONVERTION_ERROR 3
#define INT_CONVERTION_ERROR 4
#define READ_FILE_ERROR 5
#define STRING_PARSING_ERROR 6
#define WRONG_INPUT 7
#define DIRECTORY_DOES_NOT_EXIST 8
#define COULD_NOT_CREATE_DIRECTORY 9
#define DIRECTORY_ERROR 10
#define FILE_OPENING_ERROR 11
#define FILE_WRITING_ERROR 12
#define FILE_CLOSING_ERROR 13
#define WRITE_TO_FILE_ERROR 14
#define COULD_NOT_CREATE_FIFO 15
#define COULD_NOT_OPEN_FIFO 15
#define COULD_NOT_OPEN_FILE 16
#define COULD_NOT_WRITE_TO_FIFO 17
#define COULD_NOT_READ_FROM_FIFO 18
#define COULD_NOT_WRITE_TO_FILE 19
#define COULD_NOT_CLOSE_FILE 20
#define COULD_NOT_CLOSE_FIFO 21
#define COULD_NOT_LOCK_FILE 22
#define COULD_NOT_UNLOCK_FILE 23
#define NOT_FOUND 24
#define SOCKET_FAILED 25
#define SOCKET_BIND_ERROR 26
#define SOCKET_LISTEN_ERROR 27
#define SOCKET_ACCEPT_ERROR 28
#define GET_HOST_BY_ADDRESS_ERROR 29
#define BUFFER_FULL 30
#define BUFFER_EMPTY 31

#include "errno.h"
#include <stdio.h>

void print_error(int error_code, const char *message);

#endif