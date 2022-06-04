CC = gcc
CCFLAGS = -g -Wall
LIB = libpipeliner.a
LIBOBJS = pipeliner.o
BUILD = $(LIB)

all:    $(BUILD)

$(LIB): $(LIBOBJS)
	ar rcs $(LIB) $(LIBOBJS)

clean:
	rm -f $(LIB)
	rm -f $(LIBOBJS)

pipeliner.o: pipeliner.h ../terminate.h ../data_processing.h ../single_data_transfer.h ../multiply.h ../branch.h

.PHONY: all clean