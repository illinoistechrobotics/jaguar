#include <sched.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h> //needed for memcpy
#include <sys/types.h>
#include <sys/stat.h>
//#define DEBUG
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

// CANMessage structure
typedef struct{
uint8_t data[8];
uint8_t datalen;
uint8_t canid;
uint8_t mfgid;
uint8_t devid;
uint8_t apicls;
uint8_t apinum;
} CANMessage;

//MotorController structure
typedef struct{
uint8_t canid;
uint8_t error;
float din_Vbus;
float din_Vout;
float din_Ibus;
float din_temp;
uint8_t din_fault;
int16_t dout_Vout;
} MotorController;


