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

// API Classes
#define API_VOLTAGE 0
#define API_SPEED 1
#define API_VOLTAGE_COMPENSATED 2
#define API_POSITION 3
#define API_CURRENT 4
#define API_STATUS 5
#define API_CONFIG 7
#define API_ACK 8
