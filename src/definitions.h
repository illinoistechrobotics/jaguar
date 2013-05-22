#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#define SERIALDEV "/dev/ttyACM0"
int serial_file;
int serial_init=0;
