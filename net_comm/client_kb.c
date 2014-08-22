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
SDL_Window *window;                    
    SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2

    // Create an application window with the following settings:
    window = SDL_CreateWindow(
        "Control UI",                  // window title
        SDL_WINDOWPOS_UNDEFINED,           // initial x position
        SDL_WINDOWPOS_UNDEFINED,           // initial y position
        480,                               // width, in pixels
        80,                               // height, in pixels
        SDL_WINDOW_RESIZABLE                  // flags
    );

    // Check that the window was successfully made
    if (window == NULL) {
        // In the event that the window could not be made...
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

atexit(SDL_Quit);
const Uint8 *state = SDL_GetKeyboardState(NULL);
//Network Stuff
printf("Network...\n");
int sock, rv, length;
struct sockaddr_in server, from;
struct hostent *hp;
int tx,ty;
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
signal(SIGINT,quit);
while(loop){
packet.seq++;
SDL_PumpEvents();
packet.x = 0;
packet.y = 0;
if(state[SDL_SCANCODE_LEFT]){
packet.x = 0;
packet.y = -80;
} else if(state[SDL_SCANCODE_RIGHT]){
packet.x = 0;
packet.y = 80;
} else if(state[SDL_SCANCODE_UP]){
packet.x = -100;
packet.y = 0;
} else if(state[SDL_SCANCODE_DOWN]){
packet.x = 100;
packet.y = 0;
}
packet.checksum=packet.x^packet.y;
rv=sendto(sock,&packet,sizeof(packet),0,(const struct sockaddr *)&server,length);
//printf("X=%d, Y=%d\n",packet.x,packet.y);
usleep(10000);
}
 
 return 0;
}


