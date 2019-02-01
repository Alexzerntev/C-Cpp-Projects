#include "cuda_computations.cuh"

extern "C" void compute_image(uint8_t *src, size_t buf_size)
{
	uint8_t *d_src, *d_dest, *tmp;

	cudaMalloc(&d_src, buf_size * sizeof(uint8_t));
	cudaMalloc(&d_dest, buf_size * sizeof(uint8_t));
	cudaMemcpy(d_src, src, buf_size, cudaMemcpyHostToDevice);
	cudaMemset(d_dest, 0, buf_size);

	int i;
	const int block_size = 16;

	for (i = 0; i < ITERATIONS; i++)
	{
		int total_width = CURRENT_TYPE == GREY ? WIDTH : WIDTH * 3;
		int gridX = (HEIGHT + block_size - 1) / block_size;
		int gridY = (total_width + block_size - 1) / block_size;
		dim3 block(block_size, block_size);
		dim3 grid(gridX, gridY);

		if (CURRENT_TYPE == GREY)
		{
			apply_filter_grey<<<grid, block>>>(d_src, d_dest);
		}
		else
		{
			apply_filter_rgb<<<grid, block>>>(d_src, d_dest);
		}

		tmp = d_src;
		d_src = d_dest;
		d_dest = tmp;
	}

	cudaGetLastError();
	cudaDeviceSynchronize();

	if (ITERATIONS % 2 == 0)
	{
		cudaMemcpy(src, d_src, buf_size, cudaMemcpyDeviceToHost);
	}
	else
	{
		cudaMemcpy(src, d_dest, buf_size, cudaMemcpyDeviceToHost);
	}

	cudaFree(d_src);
	cudaFree(d_dest);
}

__global__ void apply_filter_grey(uint8_t *src, uint8_t *dest)
{
	int i, j, k, l;

	float blur_filter[3][3] = {{1 / 9.0 , 1 / 9.0 , 1 / 9.0}, {1 / 9.0, 1 / 9.0, 1 / 9.0}, {1 / 9.0, 1 / 9.0, 1 / 9.0}};

	size_t x = blockIdx.x*blockDim.x + threadIdx.x;
	size_t y = blockIdx.y*blockDim.y + threadIdx.y;
	if (0 < x && x < HEIGHT - 1 && 0 < y && y < WIDTH - 1)
	{
		float result = 0;
		for (i = x - 1, k = 0; i <= x + 1; i++, k++)
		{
			for (j = y - 1, l = 0; j <= y + 1; j++, l++)
			{
				result += src[WIDTH * i + j] * blur_filter[k][l];
			}
		}
		dest[WIDTH * x + y] = result;
	}
}

__global__ void apply_filter_rgb(uint8_t *src, uint8_t *dest)
{
	int i, j, k, l;

	float blur_filter[3][3] = {{1 / 9.0 , 1 / 9.0 , 1 / 9.0}, {1 / 9.0, 1 / 9.0, 1 / 9.0}, {1 / 9.0, 1 / 9.0, 1 / 9.0}};

	size_t x = blockIdx.x * blockDim.x + threadIdx.x;
	size_t y = blockIdx.y * blockDim.y + threadIdx.y;

	if (0 < x && x < HEIGHT - 1 && 0 < y && y < 3 * WIDTH - 3)
	{
		float rresult = 0, gresult = 0, bresult = 0;
		for (i = x - 1, k = 0; i <= x + 1; i++, k++)
		{
			for (j = (y * 3) - 3, l = 0; j <= (y * 3) + 3; j += 3, l++)
			{
				rresult += src[(WIDTH * 3) * i + j] * blur_filter[k][l];
				gresult += src[(WIDTH * 3) * i + j + 1] * blur_filter[k][l];
				bresult += src[(WIDTH * 3) * i + j + 2] * blur_filter[k][l];
			}
		}
		dest[WIDTH * 3 * x + (y * 3)] = rresult;
		dest[WIDTH * 3 * x + (y * 3) + 1] = gresult;
		dest[WIDTH * 3 * x + (y * 3) + 2] = bresult;
	}
}
