#include "definitions.h"
#include "can.c"
#include "motorcontroller.c"
#include "ipc.c"
int ids[4]= {2,3,4,5};
int ncontrollers=4;
MotorController mc[4];
int init(char * device){
   serial_init=0;
  // struct sched_param sp = { .sched_priority = 50 };
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
/*
if( sched_setscheduler( 0, SCHED_FIFO, &sp ) == -1 )
{
	perror("sched_setscheduler");
}
*/
   return serial_file = fd;
}
int i=0;
void readAll(){
for(i=0;i<ncontrollers;i++){
   ReadMotorController(&mc[i]);
}
}
void writeAll(){
for(i=0;i<ncontrollers;i++){
   WriteMotorController(&mc[i]);
   usleep(5000);
} 
}
void printAll(){
for(i=0;i<ncontrollers;i++){
   MotorControllerPrintf(&mc[i]);
}  
}
int main(int argc, char** argv){
   printf("%d\n",init(SERIALDEV));
int i=0;
for(i=0;i<ncontrollers;i++){
   MotorController m;
   m.canid=ids[i];
   m.dout_Vout=0;
   printf("Init status %d\n",InitMotorController(API_VOLTAGE,&m));
   ReadMotorController(&m);
   WriteMotorController(&m);
   MotorControllerPrintf(&m);
   memcpy(&mc[i],&m,sizeof(MotorController));
}
printf("Init done\n");
int c=0;
//shm_setup(ncontrollers);
while(1){

c++;

readAll();
printf("READ\n");
if(c>3){
printAll();
printf("PRINTING\n");
//shm_write(&m1,1);
c=0;
}
CANBroadcastHeartbeat();
}
   return 0;
}
