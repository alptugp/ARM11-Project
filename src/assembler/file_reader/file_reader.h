#ifndef FILE_READER
#define FILE_READER

// Takes filename of assembler file (e.g. “add01.s”) and output an array, one string for each line
// ALso strips leading and trailining whitespace
char **read_file(char filename[]);

#endif
