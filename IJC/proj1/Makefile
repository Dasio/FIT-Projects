# @file makefile
# @brief makefile
# Riesenie IJC-DU1, priklad a),b)
# @author David Mikus, FIT
# @date 13.3.2014

CC=gcc
CFLAGS=-O2 -Wall -std=c99 -pedantic

all: prvocisla prvocisla-inline steg-decode steg-encode

error.o: error.c error.h
	$(CC) $(CFLAGS) -c error.c -o error.o
eratosthenes.o: eratosthenes.c eratosthenes.h bit-array.h
	$(CC) $(CFLAGS) -c eratosthenes.c -o eratosthenes.o
ppm.o: ppm.c ppm.h
	$(CC) $(CFLAGS) -c ppm.c -o ppm.o		
	
prvocisla.o: prvocisla.c error.h eratosthenes.h 
	$(CC) $(CFLAGS) -c prvocisla.c -o prvocisla.o
prvocisla: error.o eratosthenes.o prvocisla.o
	$(CC) $(CFLAGS) error.o prvocisla.o eratosthenes.o -o prvocisla -lm	
	
prvocisla-inline.o: prvocisla.c error.h eratosthenes.h
	$(CC) $(CFLAGS) -DUSE_INLINE -c prvocisla.c -o prvocisla-inline.o
prvocisla-inline: error.o eratosthenes.o prvocisla-inline.o
	$(CC) $(CFLAGS) error.o prvocisla-inline.o eratosthenes.o -o prvocisla-inline -lm	
		
steg-decode.o: steg-decode.c error.h eratosthenes.h ppm.h
	$(CC) $(CFLAGS) -c steg-decode.c -o steg-decode.o
steg-decode: error.o eratosthenes.o ppm.o steg-decode.o
	$(CC) $(CFLAGS) error.o ppm.o eratosthenes.o steg-decode.o -o steg-decode -lm

steg-encode.o: steg-encode.c error.h eratosthenes.h ppm.h
	$(CC) $(CFLAGS) -c steg-encode.c -o steg-encode.o
steg-encode: error.o eratosthenes.o ppm.o steg-encode.o
	$(CC) $(CFLAGS) error.o ppm.o eratosthenes.o steg-encode.o -o steg-encode -lm

clean:
	rm -f *.o
	rm -f prvocisla
	rm -f prvocisla-inline
	rm -f steg-decode
	rm -f steg-encode
	rm -f xmikus15.zip
zip:
	zip xmikus15.zip *.c *.h Makefile
