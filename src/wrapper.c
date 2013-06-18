#include "definitions.h"
#include "can.c"
#include "motorcontroller.c"
#include "ipc.c"
int init(char * device){
   serial_init=0;
   struct sched_param sp = { .sched_priority = 50 };
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
if( sched_setscheduler( 0, SCHED_FIFO, &sp ) == -1 )
{
	perror("sched_setscheduler");
}
   return serial_file = fd;
}
int main(int argc, char** argv){
   printf("%d\n",init(SERIALDEV));
   MotorController m1;
   FIFOTree f1;
   m1.canid=2;
   m1.dout_Vout=0;
   printf("Init status %d\n",InitMotorController(API_VOLTAGE,&m1));
   printf("Read status %d\n",ReadMotorController(&m1));
   FIFOSetup(&m1,&f1);
   MotorControllerPrintf(&m1);
   WriteMotorController(&m1);
   MotorControllerPrintf(&m1);
int c=0;
while(1){
usleep(20000);
c++;
FIFORead(&m1,&f1);
WriteMotorController(&m1);
if(c>40){
ReadMotorController(&m1);
FIFOWrite(&m1,&f1);
MotorControllerPrintf(&m1);
CANBroadcastHeartbeat();
c=0;
}
}
   return 0;
}
