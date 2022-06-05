#ifndef BINARY_LOADER
#define BINARY_LOADER

#include "emulate.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

void file_load(char *file_path, word mem_size, memory_t memory);

#endif
