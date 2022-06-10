CC = gcc
CFLAGS = -g -Wall -pedantic -std=c99 -Ipipeliner -fsanitize=undefined
LDFLAGS = -fsanitize=undefined
LDLIBS = -Lpipeliner -lpipeliner -Lbinary_loader -lbinary_loader
BUILD = libs emulate

all:	$(BUILD)

emulate:	emulate.o emulate_utils.o

libs:
	cd pipeliner; make
	cd binary_loader; make

clean:
	rm -f *.o
	rm -f *.a
	rm -f $(BUILD)
	cd pipeliner; make clean
	cd binary_loader; make clean

.PHONY: all clean

emulate.o:	emulate.h emulate_utils.h

emulate_utils.o:	emulate_utils.h
