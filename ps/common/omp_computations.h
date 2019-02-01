#ifndef OMP_COMPUTATIONS_H
#define OMP_COMPUTATIONS_H

#include <stdint.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./constants.h"

void apply_filter(
    uint8_t *src,
    uint8_t *dest,
    int start_row,
    int end_row,
    int start_col,
    int end_col,
    int width,
    float **filter);

void compute_image(
    uint8_t **src,
    uint8_t **dest,
    int rows,
    int cols,
    float **filter,
    int top,
    int bot,
    int right,
    int left,
    MPI_Request *send_top,
    MPI_Request *send_bot,
    MPI_Request *send_left,
    MPI_Request *send_right,
    MPI_Request *recieve_top,
    MPI_Request *recieve_bot,
    MPI_Request *recieve_left,
    MPI_Request *recieve_right,
    MPI_Status *status);

#endif