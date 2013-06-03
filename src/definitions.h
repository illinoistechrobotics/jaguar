#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#define SERIALDEV "/dev/ttyAMA0"
#define MFG_TI (uint8_t)2
#define DEVTYPE_MOTORCNTL (uint8_t)2
#define SYSCTL (uint8_t)0
int serial_file;
int serial_init=0;
