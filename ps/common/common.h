#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./constants.h"

int init(
    int *buf_size,
    int *file_size,
    int process_count,
    int rows,
    int cols,
    int *top,
    int *bot,
    int *left,
    int *right,
    int *start_row,
    int *start_col,
    int process_id,
    int col_division);

int partition(int processes);

uint8_t *get_distance(uint8_t *ptr, int i, int j, int size);

void exchange_borders(
    uint8_t *src,
    int top,
    int bot,
    int right,
    int left,
    int rows,
    int cols,
    MPI_Datatype grey_col,
    MPI_Datatype rgb_col,
    MPI_Datatype grey_row,
    MPI_Datatype rgb_row,
    MPI_Request *send_top,
    MPI_Request *send_bot,
    MPI_Request *send_left,
    MPI_Request *send_right,
    MPI_Request *recieve_top,
    MPI_Request *recieve_bot,
    MPI_Request *recieve_left,
    MPI_Request *recieve_right);

void exchange_time(int process_id, int process_count, double finish, MPI_Status *status);
void load_image(uint8_t *src, int rows, int cols, int start_row, int start_col, MPI_Status *status);
void save_image(uint8_t *src, int rows, int cols, int start_col, int start_row);

#endif