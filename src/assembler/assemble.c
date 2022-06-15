#include <stdlib.h>
#include "utils.h"
#include "binary_writer/binary_writer.h"
#include "file_reader/file_reader.h"
#include "first_pass/first_pass.h"
#include "second_pass/second_pass.h"

int main(int argc, char **argv) {
  assert(argc == 3);
  char **file_lines = read_file(argv[1]);
  symbol_table_t *labels_to_addresses = initialise_symbol_table();
  char **first_pass_lines = first_pass(file_lines, labels_to_addresses);
  binary_instruction *binary_instructions = second_pass(first_pass_lines, labels_to_addresses);
  write_binary_instructions(argv[2], binary_instructions);
  free_symbol_table(labels_to_addresses);
}
