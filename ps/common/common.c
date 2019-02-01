#include "common.h"
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
    int col_division)
{
    *start_row = (process_id / col_division) * rows;
    *start_col = (process_id % col_division) * cols;
    *file_size = CURRENT_TYPE == GREY ? WIDTH * HEIGHT : WIDTH * 3 * HEIGHT;

    // to find boarders
    if (*start_row != 0)
        *top = process_id - col_division;
    if (*start_row + rows != HEIGHT)
        *bot = process_id + col_division;
    if (*start_col != 0)
        *left = process_id - 1;
    if (*start_col + cols != WIDTH)
        *right = process_id + 1;
}

int partition(int processes)
{
    int min_part = HEIGHT + WIDTH + 1;
    int part, rows_to, cols_to;
    int optimal = 0;
    for (rows_to = 1; rows_to <= processes; ++rows_to)
    {
        if (processes % rows_to || HEIGHT % rows_to)
            continue;
        cols_to = processes / rows_to;
        if (WIDTH % cols_to)
            continue;
        part = HEIGHT / rows_to + WIDTH / cols_to;
        if (part < min_part)
        {
            min_part = part;
            optimal = rows_to;
        }
    }
    return optimal;
}

uint8_t *get_distance(uint8_t *ptr, int i, int j, int size)
{
    return &ptr[size * i + j];
}

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
    MPI_Request *recieve_right)
{
    int col_size = CURRENT_TYPE == GREY ? cols + 2 : 3 * cols + 6;
    if (CURRENT_TYPE == GREY)
    {
        if (top != -1)
        {
            MPI_Isend(get_distance(src, 1, 1, col_size), 1, grey_row, top, 0, MPI_COMM_WORLD, send_top);
            MPI_Irecv(get_distance(src, 0, 1, col_size), 1, grey_row, top, 0, MPI_COMM_WORLD, recieve_top);
        }
        if (left != -1)
        {
            MPI_Isend(get_distance(src, 1, 1, col_size), 1, grey_col, left, 0, MPI_COMM_WORLD, send_left);
            MPI_Irecv(get_distance(src, 1, 0, col_size), 1, grey_col, left, 0, MPI_COMM_WORLD, recieve_left);
        }
        if (bot != -1)
        {
            MPI_Isend(get_distance(src, rows, 1, col_size), 1, grey_row, bot, 0, MPI_COMM_WORLD, send_bot);
            MPI_Irecv(get_distance(src, rows + 1, 1, col_size), 1, grey_row, bot, 0, MPI_COMM_WORLD, recieve_bot);
        }
        if (right != -1)
        {
            MPI_Isend(get_distance(src, 1, cols, col_size), 1, grey_col, right, 0, MPI_COMM_WORLD, send_right);
            MPI_Irecv(get_distance(src, 1, cols + 1, col_size), 1, grey_col, right, 0, MPI_COMM_WORLD, recieve_right);
        }
    }
    else
    {
        if (top != -1)
        {
            MPI_Isend(get_distance(src, 1, 3, col_size), 1, rgb_row, top, 0, MPI_COMM_WORLD, send_top);
            MPI_Irecv(get_distance(src, 0, 3, col_size), 1, rgb_row, top, 0, MPI_COMM_WORLD, recieve_top);
        }
        if (left != -1)
        {
            MPI_Isend(get_distance(src, 1, 3, col_size), 1, rgb_col, left, 0, MPI_COMM_WORLD, send_left);
            MPI_Irecv(get_distance(src, 1, 0, col_size), 1, rgb_col, left, 0, MPI_COMM_WORLD, recieve_left);
        }
        if (bot != -1)
        {
            MPI_Isend(get_distance(src, rows, 3, col_size), 1, rgb_row, bot, 0, MPI_COMM_WORLD, send_bot);
            MPI_Irecv(get_distance(src, rows + 1, 3, col_size), 1, rgb_row, bot, 0, MPI_COMM_WORLD, recieve_bot);
        }
        if (right != -1)
        {
            MPI_Isend(get_distance(src, 1, 3 * cols, col_size), 1, rgb_col, right, 0, MPI_COMM_WORLD, send_right);
            MPI_Irecv(get_distance(src, 1, 3 * cols + 3, col_size), 1, rgb_col, right, 0, MPI_COMM_WORLD, recieve_right);
        }
    }
}

void exchange_time(int process_id, int process_count, double finish, MPI_Status *status)
{
    int i;
    double others_finish;

    if (process_id != 0)
    {
        MPI_Send(&finish, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }
    else
    {
        for (i = 1; i < process_count; i++)
        {
            MPI_Recv(&others_finish, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, status);
            if (others_finish > finish)
            {
                finish = others_finish;
            }
        }
        printf("Time : %f\n", finish);
    }
}

void load_image(uint8_t *src, int rows, int cols, int start_row, int start_col, MPI_Status *status)
{
    int col_size = CURRENT_TYPE == GREY ? cols + 2 : 3 * cols + 6;
    MPI_File file;
    uint8_t *temp_buf;
    int i;
    MPI_File_open(MPI_COMM_WORLD, CURRENT_IMAGE_FILE, MPI_MODE_RDONLY, MPI_INFO_NULL, &file);
    if (CURRENT_TYPE == GREY)
    {
        for (i = 1; i <= rows; i++)
        {
            MPI_File_seek(file, (start_row + i - 1) * WIDTH + start_col, MPI_SEEK_SET);
            temp_buf = get_distance(src, i, 1, col_size);
            MPI_File_read(file, temp_buf, cols, MPI_BYTE, status);
        }
    }
    else if (CURRENT_TYPE == RGB)
    {
        for (i = 1; i <= rows; i++)
        {
            MPI_File_seek(file, 3 * (start_row + i - 1) * WIDTH + 3 * start_col, MPI_SEEK_SET);
            temp_buf = get_distance(src, i, 3, col_size);
            MPI_File_read(file, temp_buf, cols * 3, MPI_BYTE, status);
        }
    }
    MPI_File_close(&file);
}

void save_image(uint8_t *src, int rows, int cols, int start_col, int start_row)
{
    int col_size = CURRENT_TYPE == GREY ? cols + 2 : 3 * cols + 6;
    int i;
    uint8_t *temp_buf;
    MPI_File file;
    MPI_File_open(MPI_COMM_WORLD, RESULT_FILE_NAME, MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &file);
    if (CURRENT_TYPE == GREY)
    {
        for (i = 1; i <= rows; i++)
        {
            MPI_File_seek(file, (start_row + i - 1) * WIDTH + start_col, MPI_SEEK_SET);
            temp_buf = get_distance(src, i, 1, col_size);
            MPI_File_write(file, temp_buf, cols, MPI_BYTE, MPI_STATUS_IGNORE);
        }
    }
    else if (CURRENT_TYPE == RGB)
    {
        for (i = 1; i <= rows; i++)
        {
            MPI_File_seek(file, 3 * (start_row + i - 1) * WIDTH + 3 * start_col, MPI_SEEK_SET);
            temp_buf = get_distance(src, i, 3, col_size);
            MPI_File_write(file, temp_buf, cols * 3, MPI_BYTE, MPI_STATUS_IGNORE);
        }
    }
    MPI_File_close(&file);
}