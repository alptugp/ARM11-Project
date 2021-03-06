#include <stdlib.h>
#include "utils.h"
#include "binary_writer.h"
#include "file_reader.h"
#include "first_pass.h"
#include "second_pass.h"

int main(int argc, char **argv) {
  // Read lines from file
  assert(argc == 3);
  char **file_lines  = malloc(MAX_NUM_INSTRUCTIONS * sizeof(char *));
  int num_raw_lines = read_file(argv[1], file_lines);
  assert(file_lines);

  symbol_table_t labels_to_addresses = initialise_symbol_table(MAX_LINE_CHARS);
  char **delabelled_lines = malloc(num_raw_lines * sizeof(char *));
  int num_delabelled_lines = first_pass(file_lines, &labels_to_addresses, num_raw_lines, delabelled_lines);
  assert(delabelled_lines[0]);

  binary_instruction binary_instructions[MAX_NUM_INSTRUCTIONS];
  int num_instructions = second_pass(delabelled_lines, num_delabelled_lines, &labels_to_addresses, binary_instructions);
  write_binary_instructions(argv[2], binary_instructions, num_instructions);

  // Free heap-allocated data
  free_symbol_table(&labels_to_addresses);
  free_lines(delabelled_lines, num_delabelled_lines);
  free_lines(file_lines, num_raw_lines);
}
