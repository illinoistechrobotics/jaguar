#include "definitions.h"
#include "can.c"
#include "motorcontroller.c"
int init(char * device){
   serial_init=0;
   struct termios spconfig;
   int fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
   if(fd == -1 || !isatty(fd) || tcgetattr(fd, &spconfig) < 0) {
      return -1;
   }
   cfmakeraw(&spconfig);
   cfsetispeed(&spconfig, B115200);
   cfsetospeed(&spconfig, B115200);
   tcsetattr(fd,TCSANOW,&spconfig);
   serial_init =1;
   return serial_file = fd;
}
int main(int argc, char** argv){
   printf("%d\n",init(SERIALDEV));
   MotorController m1;
   m1.canid=2;
   m1.dout_Vout=-31000;
   printf("Init status %d\n",InitMotorController(API_VOLTAGE,&m1));
   printf("Read status %d\n",ReadMotorController(&m1));
   MotorControllerPrintf(&m1);
   WriteMotorController(&m1);
   MotorControllerPrintf(&m1);
int c=0;
while(1){
CANBroadcastHeartbeat();
usleep(50000);
c++;
if(c>40){
ReadMotorController(&m1);
MotorControllerPrintf(&m1);
c=0;
}
}
   return 0;
}
