#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <cuda.h>
#include "../common/constants.h"
#include "./cuda_computations.cuh"

int main(int argc, char **argv)
{
	int file, result_file, read_bytes = 0, write_bytes = 0, rd;
	uint8_t *src = NULL;
	int buf_size;

	cudaEvent_t start, stop;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);

	float elapsed_time = 0;

	if (CURRENT_TYPE == GREY)
	{
		buf_size = HEIGHT * WIDTH;
	}
	else
	{
		buf_size = HEIGHT * 3 * WIDTH;
	}

	//reading the input image
	if ((file = open(CURRENT_IMAGE_FILE, O_RDONLY)) < 0)
	{
		fprintf(stderr, "error in opening file %s\n", CURRENT_IMAGE_FILE);
		return -1;
	}

	src = (uint8_t *)malloc(buf_size * sizeof(uint8_t));

	for (rd = 0; rd < buf_size; rd += read_bytes)
	{
		if ((read_bytes = read(file, src + rd, buf_size - rd)) < 0)
		{
			return -1;
		}
	}

	close(file);

	cudaEventRecord(start, 0);
	
	compute_image(src, buf_size);

	cudaEventRecord(stop, 0);
	cudaEventSynchronize(stop);
	
	cudaEventElapsedTime(&elapsed_time, start, stop);
	printf("Time : %f\n", elapsed_time / 1000);
	cudaEventDestroy(start);
	cudaEventDestroy(stop);

	// saving the result
	if ((result_file = open(RESULT_FILE_NAME, O_CREAT | O_WRONLY, 0644)) < 0)
	{
		fprintf(stderr, "error in opening file  %s\n", RESULT_FILE_NAME);
		return EXIT_FAILURE;
	}

	for (rd = 0; rd < buf_size; rd += write_bytes)
	{
		if ((write_bytes = write(result_file, src + rd, buf_size - rd)) < 0)
		{
			return -1;
		}
	}
	close(result_file);

	free(src);

	return 0;
}