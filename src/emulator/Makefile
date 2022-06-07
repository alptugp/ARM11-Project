CC = gcc
CFLAGS = -g -Wall -pedantic -std=c99 -Ipipeliner -fsanitize=undefined
LDFLAGS = -fsanitize=undefined
LDLIBS = -Lpipeliner -lpipeliner
BUILD = libs emulate

all:	$(BUILD)

emulate:	main.o emulate.o binary_loader.o

libs:
	cd pipeliner; make

clean:
	rm -f *.o
	rm -f *.a
	rm -f $(BUILD)
	cd pipeliner; make clean

.PHONY: all clean

main.o:	emulate.h binary_loader.h

emulate.o:	emulate.h

binary_loader.o:	binary_loader.h
