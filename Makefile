CC=gcc
CFLAGS=-Wall -g -o 
LDFLAGS=-lm
all:
	$(CC) $(CFLAGS) search search.c $(LDFLAGS)
clean:
	rm -rf search
