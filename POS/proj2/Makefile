CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -pedantic -g
LFLAGS=-lpthread
BIN=shell

program: main.c
	$(CC) $(CFLAGS) $^ -o $(BIN) $(LFLAGS)

clean:
	rm -f $(BIN)

.PHONY: clean
