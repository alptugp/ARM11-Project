NAME = emulate
CC = gcc
CFLAGS = -g -Wall -pedantic -std=c99 -fsanitize=undefined
LDFLAGS = -fsanitize=undefined
LIBS = pipeliner binary_loader
LDLIBS = $(foreach lib, $(LIBS), -L$(lib) -l$(lib))
BUILD = libs $(NAME)

all:	$(BUILD)

$(NAME):	$(NAME).o emulate_utils.o

libs:
	$(foreach lib, $(LIBS), cd $(lib); make; cd ..;)

clean:
	rm -f *.o
	rm -f *.a
	rm -f $(BUILD)
	$(foreach lib, $(LIBS), cd $(lib); make clean; cd ..;)

.PHONY: all clean libs

$(NAME).o:	$(NAME).h emulate_utils.h

emulate_utils.o:	emulate_utils.h
