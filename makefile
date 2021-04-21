CC = gcc
CFLAGS = -Wall -std=c99

system.o: system.c system.h
	$(CC) $(CFLAGS) -c system.c

main.o: system.h main.c
	$(CC) $(CFLAGS) -c main.c

main: system.o main.o
	$(CC) $(CFLAGS) -o main main.o system.o
