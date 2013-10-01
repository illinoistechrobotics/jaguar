#include "definitions.h"
#include "can.c"
#include "motorcontroller.c"
#include <sys/shm.h>
char *shm_pointer;

void shm_setup(uint8_t ndevs){
		int shmid;
		key_t key;
		key = 1339;
        uint16_t size = 2+(sizeof(MotorController)*ndevs);
        if ((shmid = shmget(key, size, 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
		if ((shm_pointer = shmat(shmid, NULL, 0)) == (char *) -1) {
				perror("shmat");
				exit(1);
		}
}
MotorController *temp;
int i=0;
int main(int argc, char ** argv){
    shm_setup(1);
for(;;){
		for(i=0; i<1; i++){
				temp=(MotorController *)(shm_pointer+2+(i*sizeof(MotorController)));
				MotorControllerPrintf(temp);
				usleep(100000);
		}
}
return 0;

}
