#ifndef FILE_READER
#define FILE_READER

#include "../utils.h"

// Takes filename of assembler file (e.g. “add01.s”) and output an array, one string for each line
// ALso strips leading and trailining whitespace
int read_file(char filename[], char **file_lines);

#endif
