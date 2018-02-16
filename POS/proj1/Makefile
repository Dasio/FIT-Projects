CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -pedantic
LFLAGS=-lpthread
BIN=program

program: main.c
	$(CC) $(CFLAGS) $^ -o $(BIN) $(LFLAGS)

clean:
	rm -f $(BIN)

.PHONY: clean
