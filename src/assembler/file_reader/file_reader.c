#include <string.h>
#include "file_reader.h"

int read_file(char *filename, char **file_lines) {

    FILE *fp = fopen(filename, "r");
    if(!fp) {
        printf("Couldn't open asm filename.\n");
        assert(0);
    }

    int line_count = 0;
    char line[MAX_LINE_CHARS + 2]; // add 1 to include null chars
    assert(file_lines);

    while(fgets(line, MAX_LINE_CHARS + 2, fp)) {
        line[strcspn(line, "\n")] = '\0'; // end line beginning from newline
        file_lines[line_count] = (char *) malloc(MAX_LINE_CHARS);
        assert(file_lines[line_count]);
        strncpy(file_lines[line_count], line, MAX_LINE_CHARS);
        line_count++;
    }

    fclose(fp);
    return line_count;
}
