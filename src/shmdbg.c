#include "definitions.h"
#include "can.c"
#include "motorcontroller.c"
#include <sys/shm.h>
#include "ipc.c"
MotorController *temp;
int i=0, dev=0;
int main(int argc, char ** argv){
    dev=shm_connect();
    printf("%d devices found\n",dev);
for(;;){
	for(i=0; i<4; i++){
			MotorController mc;;
			memcpy(&mc,shm_pointer+2+(i*sizeof(MotorController)), sizeof(MotorController));
			MotorControllerPrintf(&mc);
				usleep(100000);
		}
}
return 0;

}
