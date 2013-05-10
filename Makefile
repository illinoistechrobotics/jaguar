#Makefile for jaguar driver
CC = gcc
CFLAGS = -Wall -Werror -g

testing:;       $(CC) $(CFLAGS) -o test src/serialio.c

clean:;        rm -f test
