#define BINARY_LOADER.H

#include "emulate.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

FILE *file_load(char *file_path, word mem_size);
