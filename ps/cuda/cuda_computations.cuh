#ifndef CUDA_CONVOLUTE
#define CUDA_CONVOLUTE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "../common/constants.h"

extern "C" void compute_image(uint8_t *src, size_t buf_size);
__global__ void apply_filter_grey(uint8_t *src, uint8_t *dest);
__global__ void apply_filter_rgb(uint8_t *src, uint8_t *dest);

#endif