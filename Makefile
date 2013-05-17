#Makefile for jaguar driver
CC = gcc
CFLAGS = -Wall -Werror -g

testing:;       $(CC) $(CFLAGS) -o test src/wrapper.c

clean:;        rm -f test
