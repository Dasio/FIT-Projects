CC = gcc
CFLAGS = -std=gnu99 -Wall -Wextra -Werror -pedantic
LFLAGS = -lrt -lpthread

rivercrossing: rivercrossing.o
	$(CC) $(CFLAGS) $< -o $@ $(LFLAGS)
debug : CFLAGS += -g
debug : rivercrossing 

%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $<
clean :
	rm -f rivercrossing	rivercrossing.o

