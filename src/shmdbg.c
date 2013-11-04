#include "definitions.h"
#include "can.c"
#include "motorcontroller.c"
#include <sys/shm.h>
#include "ipc.c"
MotorController *temp;
int i=0, dev=0;
MotorController *mcg;
int main(int argc, char ** argv){
	mcg=(MotorController*)malloc(dev*sizeof(MotorController));   
	dev=shm_connect();
	printf("%d devices found\n",dev);
	for(;;){

		shm_cli_read(mcg,dev);
		shm_cli_write(mcg,dev);
		for(i=0; i<dev; i++){
			MotorControllerPrintf(&mcg[i]);
			mcg[i].dout_Vout=0;


			usleep(100000);
		}
	}
	return 0;

}
