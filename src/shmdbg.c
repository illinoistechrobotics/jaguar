#include "definitions.h"
#include "can.c"
#include "motorcontroller.c"
#include <sys/shm.h>
#include "ipc.c"
MotorController *temp;
int i=0;
int main(int argc, char ** argv){
    shm_connect();
    printf("%d devices found\n",ndevs);
for(;;){
		for(i=0; i<ndevs; i++){
				temp=(MotorController *)(shm_pointer+2+(i*sizeof(MotorController)));
				MotorControllerPrintf(temp);
				usleep(1000000);
		}
}
return 0;

}
