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
   int n;
   printf("%d\n",init("/dev/ttyACM0"));
   CANSendMessage(CreateMessageID(1,9,0,0),NULL,0);
   usleep(100000);
   n=read(serial_file,buf,50);
   write(STDOUT_FILENO,buf,n);
   return 0;
}
