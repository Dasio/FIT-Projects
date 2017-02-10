CC=gcc
CP=g++-4.8
CFLAGS= -Wall -Wextra -pedantic -std=c99
CPFLAGS= -std=c++11 -pedantic -Wall
LIB=hash_function.o htab_init.o htab_lookup.o htab_foreach.o htab_remove.o htab_clear.o htab_free.o htab_statistics.o htab_custom.o

all: fold fold2 wordcount wordcount-dynamic
debug: CFLAGS += -g
debug: all

fold: fold.c io.o
	$(CC) $(CFLAGS) io.o fold.c -o fold
fold2: fold2.cc
	$(CP) $(CPFLAGS) fold2.cc -o fold2
wordcount:wordcount.o io.o libhtable.a
	$(CC) $(CFLAGS) -static io.o wordcount.o libhtable.a -o $@
wordcount-dynamic: wordcount.o io.o libhtable.so
	$(CC) $(CLAFGS) -Wl,-rpath,. wordcount.o io.o libhtable.so -L. ./libhtable.so -o $@ 


libhtable.a: $(LIB)
	ar crs libhtable.a $(LIB)
libhtable.so: $(LIB)
	$(CC) $(CFLAGS) -shared -fPIC $(LIB) -o libhtable.so
	
io.o: io.c 
	$(CC) $(CFLAGS) -c io.c
wordcount.o: wordcount.c
	$(CC) $(CFLAGS) -c wordcount.c
#htab_*
%.o : %.c 
	$(CC) $(CFLAGS) -fPIC -c $<
clean:
	rm -f fold
	rm -f fold2
	rm -f wordcount
	rm -f wordcount-dynamic
	rm -f libhtable.a
	rm -f libhtable.so
	rm -f *.o
