#include "binary_loader.h"
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

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
        word closed;
        do {
        }while((closed = fclose(file)));
        assert(0);
    }

    int elems = 1;
    FILE *file_ptr = fopen(file_path, "rb");
    fseek(file_ptr, 0, SEEK_END);
    file_size = ftell(file_ptr);
    rewind(file_ptr);

    //ensures buffer, which holds the array of bytes, has enough memory for the file
    char *buffer = (char *)malloc(file_size);
    assert(buffer);
    fread(buffer, file_size, elems, file_ptr);
    fclose(file_ptr);

    //buffer now has array of bytes holding the file's content

    memcpy(memory, buffer, file_size);

    free(buffer);

    return file_size / sizeof(word);
}
