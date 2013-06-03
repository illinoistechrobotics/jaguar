#include "definitions.h"
#include "can.c"
int init(char * device){
   serial_init=0;
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
int main(int argc, char** argv){
   char buf[50];
   int n,i;
   printf("%d\n",init(SERIALDEV));
while(1){
   CANSendMessage(CreateMessageID(2,2,0,2,2),NULL,0);
   usleep(50000);
   n=read(serial_file,buf,50);
for(i=0;i<n;i++){
   printf("Response 1: %x\r\n",buf[i]);
}
}
   return 0;
}
