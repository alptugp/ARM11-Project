#include "binary_writer.h"
#include <string.h>

void write_binary_instructions(char filename[], binary_instruction *buffer, int num_instructions) {
    FILE *fp = fopen(filename, "w");
    if(!fp) {
        printf("Binary out file not found.\n");
        assert(0);
    }

    if(fwrite(buffer, sizeof(binary_instruction), num_instructions, fp) < num_instructions) {
        printf("Error writing to binary file.\n");
        assert(0);
    }    

    fclose(fp);

}
