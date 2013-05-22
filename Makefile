#Makefile for jaguar driver
CC = gcc
CFLAGS = -Wall -Werror -g

testing: src/wrapper.c src/can.c src/serialio.c
	$(CC) $(CFLAGS) -o test src/wrapper.c 

clean:;        rm -f test
