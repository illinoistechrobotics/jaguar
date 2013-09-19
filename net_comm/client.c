#include <stdio.h>
#include "SDL.h"
#include <stdint.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
typedef struct{
		int8_t x,y;
		char auth[9];
		uint16_t seq;
		uint8_t checksum;
} packet_t;

void error(char *msg){
  perror(msg);
  exit(1);
}
int8_t loop=1;

void quit(){
loop=0;
}
int main(int argc, char* argv[]){
if (argc != 4) { printf("Usage: server port auth\n");
                    exit(1);
   }
//SDL Stuff
printf("SDL...\n");

SDL_Joystick *joy;
SDL_Init(SDL_INIT_JOYSTICK);
// Initialize the joystick subsystem
signal(SIGINT,quit);
// Check for joystick
if (SDL_NumJoysticks() > 0) {
    // Open joystick
    joy = SDL_JoystickOpen(0);

    if (joy) {
        printf("Opened Joystick 0\n");
        printf("Name: %s\n", SDL_JoystickNameForIndex(0));
        printf("Number of Axes: %d\n", SDL_JoystickNumAxes(joy));
        printf("Number of Buttons: %d\n", SDL_JoystickNumButtons(joy));
        printf("Number of Balls: %d\n", SDL_JoystickNumBalls(joy));
    } else {
        printf("Couldn't open Joystick 0\n");
		return -1;
    }
//Network Stuff
printf("Network...\n");
int sock, rv, length;
struct sockaddr_in server, from;
struct hostent *hp;
char buffer[256];
   sock= socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) error("socket");

   server.sin_family = AF_INET;
   hp = gethostbyname(argv[1]);
   if (hp==0) error("Unknown host");

   memmove(&server.sin_addr,hp->h_addr,hp->h_length);   
   server.sin_port = htons(atoi(argv[2]));
   length=sizeof(struct sockaddr_in);

packet_t packet, sync;
if(strlen(argv[3])>8){
error("Auth string too long!");
}
memcpy(&packet.auth,argv[3],strlen(argv[3])+1);
packet.seq=65530;
while(loop&&SDL_JoystickGetAttached(joy)){
packet.seq++;
SDL_JoystickUpdate();
packet.x = SDL_JoystickGetAxis(joy, 0)/256;
packet.y = SDL_JoystickGetAxis(joy, 1)/256;
packet.checksum=packet.x^packet.y;
rv=sendto(sock,&packet,sizeof(packet),0,(const struct sockaddr *)&server,length);
//printf("X=%d, Y=%d\n",packet.x,packet.y);
usleep(10000);
}
 	 
    // Close if opened
    if (SDL_JoystickGetAttached(joy)) {
        SDL_JoystickClose(joy);
    }

}
 return 0;
}


