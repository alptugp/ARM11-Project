#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "../utils.h"



#define MAX_NUMBER_TOKENS 100
#define MAX_LINE_CHRACTERS 511
#define MAX_TOKEN_LENGTH 100
#define TOKEN_DELIMITERS " ,"

tokenized_source_code tokenize_line(char* line);
// tokenized_source_code* tokenize_file(char** input);
