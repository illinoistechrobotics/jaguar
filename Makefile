#Makefile for jaguar driver
CC = gcc
CFLAGS = -Wall -Werror -g

jaguar: src/wrapper.c src/can.c src/serialio.c
	$(CC) $(CFLAGS) -o jaguarctl src/wrapper.c 

clean:;        rm -f test
