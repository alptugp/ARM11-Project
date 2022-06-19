#include "second_pass_utils.h"
#include <string.h>

void free_lines(char **lines, int num_lines) {
  for(int i = 0; i < num_lines; i++) {
      free(lines[i]);
  }
  free(lines);
}

word get_register_address(char *register_string) {
    return strtol(&register_string[1], (char **) NULL, 0);
}

SHIFT_TYPE get_shift_type(char *shift_string) {
    if (!strcmp(shift_string, "asr")) {
        return ASR;
    } else if (!strcmp(shift_string, "lsl")) {
        return LSL;
    } else if (!strcmp(shift_string, "ror")) {
        return ROR;
    } else if (!strcmp(shift_string, "lsr")) {
        return LSR;
    } else {
        printf("No corresponding shift type found\n");
        exit(EXIT_FAILURE);
    }
}
