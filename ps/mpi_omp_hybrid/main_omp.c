#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include "mpi.h"
#include "../common/constants.h"
#include "../common/common.h"
#include "../common/mpi_computations.h"

int main(int argc, char **argv)
{
    // Init
    int i, j, k, t, start_row, start_col, row_division, col_division, rows, cols,
        file_desc, top = -1, bot = -1, left = -1, right = -1, file_size, buf_size, process_id, process_count;
    double start, finish;
    uint8_t *src, *dest;

    // MPI Init
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &process_count);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_id);

    // MPI types init
    MPI_Status status;
    MPI_Datatype grey_col;
    MPI_Datatype rgb_col;
    MPI_Datatype grey_row;
    MPI_Datatype rgb_row;
    MPI_Request send_top;
    MPI_Request send_bot;
    MPI_Request send_left;
    MPI_Request send_right;
    MPI_Request recieve_top;
    MPI_Request recieve_bot;
    MPI_Request recieve_left;
    MPI_Request recieve_right;

    MPI_Bcast(&row_division, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&col_division, 1, MPI_INT, 0, MPI_COMM_WORLD);

    row_division = partition(process_count);
    col_division = process_count / row_division;
    rows = HEIGHT / row_division;
    cols = WIDTH / col_division;

    MPI_Type_vector(rows, 1, cols + 2, MPI_BYTE, &grey_col);
    MPI_Type_commit(&grey_col);
    MPI_Type_vector(rows, 3, 3 * cols + 6, MPI_BYTE, &rgb_col);
    MPI_Type_commit(&rgb_col);

    MPI_Type_contiguous(cols, MPI_BYTE, &grey_row);
    MPI_Type_commit(&grey_row);
    MPI_Type_contiguous(3 * cols, MPI_BYTE, &rgb_row);
    MPI_Type_commit(&rgb_row);

    //sharpen filter defenition
    float blur_filter[3][3] = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};
    float **filter = malloc(3 * sizeof(float *));
    for (i = 0; i < 3; i++)
    {
        filter[i] = malloc(3 * sizeof(float));
    }
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            filter[i][j] = blur_filter[i][j] / 9.0;
        }
    }

    init(&buf_size,
         &file_size,
         process_count,
         rows,
         cols,
         &top,
         &bot,
         &left,
         &right,
         &start_row,
         &start_col,
         process_id,
         col_division);

    int col_size = CURRENT_TYPE == GREY ? cols + 2 : cols * 3 + 6;
    src = malloc(((rows + 2) * col_size) * sizeof(uint8_t));
    dest = malloc(((rows + 2) * col_size) * sizeof(uint8_t));

    load_image(src, rows, cols, start_row, start_col, &status);

    MPI_Barrier(MPI_COMM_WORLD);

    // start time mesurment
    start = MPI_Wtime();

    for (i = 0; i < ITERATIONS; i++)
    {
        // Exchange borders info
        exchange_borders(
            src, top,
            bot, right,
            left, rows,
            cols, grey_col,
            rgb_col,
            grey_row,
            rgb_row,
            &send_top,
            &send_bot,
            &send_left,
            &send_right,
            &recieve_top,
            &recieve_bot,
            &recieve_left,
            &recieve_right);

        compute_image(
            &src, &dest,
            rows, cols,
            filter, top,
            bot, right,
            left,
            &send_top,
            &send_bot,
            &send_left,
            &send_right,
            &recieve_top,
            &recieve_bot,
            &recieve_left,
            &recieve_right,
            &status);
    }

    //end time mesurment
    finish = MPI_Wtime() - start;
    exchange_time(process_id, process_count, finish, &status);

    // saving the result
    save_image(src, rows, cols, start_col, start_row);

    // House keeping
    MPI_Type_free(&rgb_col);
    MPI_Type_free(&rgb_row);
    MPI_Type_free(&grey_col);
    MPI_Type_free(&grey_row);
    free(src);
    free(dest);
    for (int i = 0; i < 3; i++)
    {
        float *currentIntPtr = filter[i];
        free(currentIntPtr);
    }

    MPI_Finalize();
    return 0;
}
