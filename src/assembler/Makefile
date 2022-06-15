CC = gcc
CFLAGS = -g -Wall -pedantic -std=c99 -fsanitize=undefined
LIBS = binary_writer first_pass second_pass symbol_table file_reader
LDFLAGS = -fsanitize=undefined
LDLIBS = $(foreach lib, $(LIBS), -L$(lib) -l$(lib))

BUILD = libs assemble

all:	$(BUILD)

assemble:	assemble.o

libs:
	$(foreach lib, $(LIBS), cd $(lib); make; cd ..;)

clean:
	rm -f *.o
	rm -f *.a
	rm -f $(BUILD)
	$(foreach lib, $(LIBS), cd $(lib); make clean; cd ..;)

.PHONY: all clean libs

assemble.o:	assemble.h