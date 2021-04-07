CC = gcc
CFLAGS = -Wall -std=c99 -g

bankManager.o: bankManager.h bankManager.c 
	$(CC) $(CFLAGS) -c bankManager.c

client.o: bankManager.h client.c 
	$(CC) $(CFLAGS) -c client.c

client: bankManager.o client.o
	$(CC) $(CFLAGS) -o client bankManager.o client.o
