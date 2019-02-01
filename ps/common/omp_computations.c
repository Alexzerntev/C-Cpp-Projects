#include "./omp_computations.h"

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
    MPI_Status *status)
{
    uint8_t *temp;
    apply_filter(*src, *dest, 1, rows, 1, cols, cols, filter);

    if (top != -1)
    {
        MPI_Wait(recieve_top, status);
        apply_filter(*src, *dest, 1, 1, 2, cols - 1, cols, filter);
    }
    if (left != -1)
    {
        MPI_Wait(recieve_left, status);
        apply_filter(*src, *dest, 2, rows - 1, 1, 1, cols, filter);
    }
    if (bot != -1)
    {
        MPI_Wait(recieve_bot, status);
        apply_filter(*src, *dest, rows, rows, 2, cols - 1, cols, filter);
    }
    if (right != -1)
    {
        MPI_Wait(recieve_right, status);
        apply_filter(*src, *dest, 2, rows - 1, cols, cols, cols, filter);
    }

    // Corners
    if (top != -1 && left != -1)
        apply_filter(*src, *dest, 1, 1, 1, 1, cols, filter);
    if (left != -1 && bot != -1)
        apply_filter(*src, *dest, rows, rows, 1, 1, cols, filter);
    if (bot != -1 && right != -1)
        apply_filter(*src, *dest, rows, rows, cols, cols, cols, filter);
    if (right != -1 && top != -1)
        apply_filter(*src, *dest, 1, 1, cols, cols, cols, filter);

    // Sync
    if (top != -1)
        MPI_Wait(send_top, status);
    if (left != -1)
        MPI_Wait(send_left, status);
    if (bot != -1)
        MPI_Wait(send_bot, status);
    if (right != -1)
        MPI_Wait(send_right, status);

    temp = *src;
    *src = *dest;
    *dest = temp;
}

void apply_filter(
    uint8_t *src,
    uint8_t *dest,
    int start_row,
    int end_row,
    int start_col,
    int end_col,
    int width,
    float **filter)
{
    int i, j, k, l, m, n;
    if (CURRENT_TYPE == GREY)
    {
        width = width + 2;
        float result = 0;
#pragma omp parallel for shared(src, dst) schedule(static) collapse(3)
        for (i = start_row; i <= end_row; i++)
        {
            for (j = start_col; j <= end_col; j++)
            {
                result = 0;
                for (k = i - 1, m = 0; k <= i + 1; k++, m++)
                {
                    for (l = j - 1, n = 0; l <= j + 1; l++, n++)
                    {
                        result += src[width * k + l] * filter[m][n];
                    }
                }
                dest[width * i + j] = result;
            }
        }
    }
    else if (CURRENT_TYPE == RGB)
    {
        width = width * 3 + 6;
        float rresult = 0, gresult = 0, bresult = 0;
#pragma omp parallel for shared(src, dst) schedule(static) collapse(3)
        for (i = start_row; i <= end_row; i++)
        {
            for (j = start_col; j <= end_col; j++)
            {
                rresult = 0;
                gresult = 0;
                bresult = 0;
                for (k = i - 1, m = 0; k <= i + 1; k++, m++)
                {
                    for (l = (j * 3) - 3, n = 0; l <= (j * 3) + 3; l += 3, n++)
                    {
                        rresult += src[width * k + l] * filter[m][n];
                        gresult += src[width * k + l + 1] * filter[m][n];
                        bresult += src[width * k + l + 2] * filter[m][n];
                    }
                }
                dest[width * i + (j * 3)] = rresult;
                dest[width * i + (j * 3) + 1] = gresult;
                dest[width * i + (j * 3) + 2] = bresult;
            }
        }
    }
}