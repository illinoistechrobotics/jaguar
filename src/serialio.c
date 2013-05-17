#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#define SERIALDEV "/dev/ttyACM0"
int main(int argc, char** argv){
  int fd;
  struct termios spconfig;
 const char* device = SERIALDEV;
 const char on = '1';
 const char off = '0';
fd = open(device, O_RDWR | O_NOCTTY);
if(fd == -1 || !isatty(fd) || tcgetattr(fd, &spconfig) < 0) {
  printf( "failed to open port %s \n",device);
  exit(EXIT_FAILURE);
}
cfmakeraw(&spconfig);
cfsetispeed(&spconfig, B115200);
cfsetospeed(&spconfig, B115200);
tcsetattr(fd,TCSANOW,&spconfig);
printf("Initializing Connection...\n");
sleep(2);
while(1){
write(fd,&on,1);
//printf("On\n");
usleep(50000);
write(fd,&off,1);
//printf("Off\n");
usleep(50000);
}
close(fd);
  return 0;
}
