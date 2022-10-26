CC=gcc
CFLAGS=-I.
DEPS = data.c data.h window.c window.h main.c main.h
OBJ = data.o window.o main.o


%.o: %.c $(DEPS)
	$(CC) -lm -pthread -g -c -o $@ $< $(CFLAGS) -lncurses 

mytop: $(OBJ)
	$(CC) -lm -pthread -g -o $@ $^ $(CFLAGS) -lncurses
