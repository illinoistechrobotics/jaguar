#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
uint8_t loop=1;
typedef struct{
		uint8_t type;
		int16_t x,y;
		char auth[9];
		uint8_t seq;
		int16_t checksum;
} packet_t;
void quit(){
		loop=0;
}
void error(char *msg){
		perror(msg);
		exit(1);
}

int main(int argc, char**argv)
{
		if(argc != 2){
				printf("incorrect number of arguments\n");
				return -1;
		}
		int sock,valid=0;
		struct sockaddr_in servaddr,cliaddr;
		packet_t packet;
		socklen_t len;
		sock=socket(AF_INET,SOCK_DGRAM,0);
		len=sizeof(servaddr);
		memset(&servaddr,'\0',len);
		servaddr.sin_family = AF_INET;
		servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
		servaddr.sin_port=htons(atoi(argv[1]));
		bind(sock,(struct sockaddr *)&servaddr,len);
		while(sock&&loop){
			recvfrom(sock,&packet,sizeof(packet),0,(struct sockaddr *)&servaddr,&len);
			if((packet.x^packet.y^packet.seq)==packet.checksum){
				valid=1;
				}
			else{
				valid=0;
			}
			if(valid){
			//Handle data here
			printf("X=%d, Y=%d\n",packet.x,packet.y);
		}
}
}
