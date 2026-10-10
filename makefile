CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic -D_POSIX_C_SOURCE=200809L
TARGETS = server client

all: $(TARGETS)

.PHONY: all clean

server: server.o
	$(CC) $(CFLAGS) server.o -o server

server.o: server.c
	$(CC) $(CFLAGS) -c server.c -o server.o

client: client.o
	$(CC) $(CFLAGS) client.o -o client

client.o: client.c
	$(CC) $(CFLAGS) -c client.c -o client.o

clean: 
	rm -f *.o $(TARGETS)