#include "binary_loader.h"
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

/*
need to check if file exists
if it does, open it
otherwise print error message saying file doesn't exist, return out of function
-
if got to this stage, file has been opened
check if size is larger than the memory size parameter passed into fn
if this is the case, close file and print error message. return out of the fn
(closing the file is the code I haven't written yet)

return the file
*/

int file_load(const char *file_path, word mem_size, memory_t memory) {
    FILE *file;
    if (access(file_path, F_OK) == 0) {
        //open file in binary mode
        file = fopen(file_path, "rb");
    } else {
        printf("File doesn't exist!\n");
        assert(0);
    }

    //jump to beginning of file
    fseek(file, 0, SEEK_SET);
    word file_size = (word) ftell(file);
    //jump to end of file
    fseek(file, 0, SEEK_END);

    if (file_size > mem_size) {
        printf("Size of the file is too large.\n");

        //closes file
        word closed = fclose(file);
        while (closed != 0) {
            closed = fclose(file);
        }
        assert(0);
    }

    int elems = 1;
    FILE *file_ptr = fopen(file_path, "rb");
    fseek(file_ptr, 0, SEEK_END);
    file_size = ftell(file_ptr);
    rewind(file_ptr);

    //ensures buffer, which holds the array of bytes, has enough memory for the file
    const int bytes_in_file = sizeof(char) * file_size;
    char *buffer = (char *)malloc(bytes_in_file);
    assert(buffer);
    fread(buffer, file_size, elems, file_ptr);
    fclose(file_ptr);

    //buffer now has array of bytes holding the file's contents

    strcpy(memory, buffer);
    
    free(buffer);

    return bytes_in_file / (sizeof(word) / sizeof(char));
}
