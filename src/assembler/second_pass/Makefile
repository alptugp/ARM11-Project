NAME = second_pass
CC = gcc
CFLAGS = -g -Wall -pedantic -std=c99
LDFLAGS = $(CFLAGS)
LIB = lib$(NAME).a
LIBOBJS = $(NAME).o branch.o data_processing.o multiply.o single_data_transfer.o tokenizer.o second_pass_utils.o
BUILD = $(LIB)

all:    $(BUILD)

$(LIB): $(LIBOBJS)
	ar rcs $(LIB) $(LIBOBJS)

clean:
	rm -f $(LIB)
	rm -f $(LIBOBJS)
	rm -f *.a
	rm -f *.o

$(NAME).o: $(NAME).h

branch.o:	branch.h
data_processing.o:	data_processing.h second_pass_utils.h
multiply.o:	multiply.h second_pass_utils.h
single_data_transfer.o:	single_data_transfer.h data_processing.h second_pass_utils.h
tokenizer.o:	tokenizer.h
second_pass_utils.o:	second_pass_utils.h

.PHONY: all clean
