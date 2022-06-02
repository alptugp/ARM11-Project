#include "binary_loader.h"

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

FILE *file_load(char *file_path, word mem_size) {
    if (access(file_path, F_OK) == 0) {
        FILE *file = fopen(file_path, "rb");
    } else {
        printf("File doesn't exist!\n")
        return NULL;
    }

    //find size of file 
    fseek(file, 0, SEEK_SET);
    word file_size = (word) ftell(file);
    fseek(file, 0, SEEK_END);

    if (file_size <= mem_size) {
        return file;
    } else {
        printf("Size of the file is too large.\n";)

        //closes file
        word closed = fclose(file);
        while (closed != 0) {
            closed = fclose(file);
        }
        return NULL;
    }
    return file;
}


