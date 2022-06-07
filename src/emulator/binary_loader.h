#ifndef BINARY_LOADER
#define BINARY_LOADER

#include "emulate.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

int file_load(const char *file_path, word mem_size, memory_t memory);

#endif
