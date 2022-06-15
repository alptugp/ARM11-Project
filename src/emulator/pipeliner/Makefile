NAME = pipeliner
CC = gcc
CCFLAGS = -g -Wall -pedantic -std=c99 -fsanitize=undefined
LDFLAGS = -fsanitize=undefined
LIB = lib$(NAME).a
LIBOBJS = $(NAME).o
BUILD = $(LIB)

all:    $(BUILD)

$(LIB): $(LIBOBJS)
	ar rcs $(LIB) $(LIBOBJS)

clean:
	rm -f $(LIB)
	rm -f $(LIBOBJS)
	rm -f *.a
	rm -f *.o

$(NAME).o: $(NAME).h \
	 ../instructions/data_processing.h ../instructions/single_data_transfer.h ../instructions/multiply.h ../instructions/branch.h ../emulate_utils.h \
	 ../instructions/data_processing.c ../instructions/single_data_transfer.c ../instructions/multiply.c ../instructions/branch.c \
	 ../instructions/gpio.h ../instructions/gpio.c

.PHONY: all clean
