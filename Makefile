#Makefile for jaguar driver
CC = gcc
CFLAGS = -Wall -Werror -g

jaguar: src/*.c
	$(CC) $(CFLAGS) -o jaguarctl src/wrapper.c

shmdbg: src/shmdbg.c
	$(CC) $(CFLAGS) -o shm_test src/shmdbg.c  
canutil: src/canutil.c
	$(CC) $(CFLAGS) -o canutil src/canutil.c
clean:;        rm -f jaguarctl
