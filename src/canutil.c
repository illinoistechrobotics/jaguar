#include "definitions.h"
#include "can.c"
#include "ipc.c"
int init(char * device){
   serial_init=0;
  // struct sched_param sp = { .sched_priority = 50 };
   struct termios spconfig;
   int fd = open(device, O_RDWR | O_NOCTTY);
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


int main(int argc, char **argv){
printf("%d\n",init(SERIALDEV));
CANMessage cm;
CANBroadcastHeartbeat();
cm.mfgid=0;
cm.canid=0;
cm.datalen=1;
cm.data[0]=atoi(argv[1]);
cm.devid=0;
cm.apinum=2;
cm.apicls=0;
CANPrintf(&cm);
CANSendMessage(&cm);
tcdrain(serial_file);
int i;
for(i=0;i<100;i++){
usleep(50000);
CANBroadcastHeartbeat();
}
return 0;
}
