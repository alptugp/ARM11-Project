#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "./utils.h"



#define MAX_NUMBER_TOKENS 100
#define MAX_LINE_CHRACTERS 511
#define MAX_TOKEN_LENGTH 100

char* get_label(char* line);
bool is_label(char *line);
char* trim_whitespace(char* str);
char* remove_comments(char* str);
bool is_label(char *line);
char* get_label(char* line);
tokenized_source_code tokenize_line(char* line);
tokenized_source_code* tokenize_file(char** input);
