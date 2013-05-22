#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include "definitions.h"
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
   printf("%d\n",init("/dev/ttyACM0"));
   return 0;
}
