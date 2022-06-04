#ifndef BINARY_LOADER
#define BINARY_LOADER

#include "emulate.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

char *file_load(char *file_path, word mem_size);

#endif
