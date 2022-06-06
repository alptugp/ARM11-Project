CC = gcc
CCFLAGS = -g -Wall -pedantic -std=c99 # -fsanitize=undefined
LIB = libpipeliner.a
LIBOBJS = pipeliner.o
BUILD = $(LIB)

all:    $(BUILD)

$(LIB): $(LIBOBJS)
	ar rcs $(LIB) $(LIBOBJS)

clean:
	rm -f $(LIB)
	rm -f $(LIBOBJS)
	rm -f *.a
	rm -f *.o

pipeliner.o: pipeliner.h \
	 ../terminate.h ../data_processing.h ../single_data_transfer.h ../multiply.h ../branch.h ../emulate.h \
	 ../terminate.c ../data_processing.c ../single_data_transfer.c ../multiply.c ../branch.c

.PHONY: all clean