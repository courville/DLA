CC=gcc-6
CFLAGS=-march=native -O3 -pipe -fomit-frame-pointer
LDFLAGS=
EXEC=dla
SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)

program_INCLUDE_DIRS := 
program_LIBRARY_DIRS :=
program_LIBRARIES := mylib

LDLIBS = -lm

all: $(EXEC)

%.o: %.c
	    @$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	    @rm -rf *.o

mrproper: clean
	    @rm -rf $(EXEC)
