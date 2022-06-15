NAME = second_pass
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

$(NAME).o: $(NAME).h

.PHONY: all clean
