#include <sys/ipc.h>
#include <sys/shm.h>

char *shm_pointer;
void shm_setup(uint8_t ndevs){
    int shmid;
    key_t key;
    key = 1339;
	uint16_t size = 2+(sizeof(MotorController)*ndevs);
    /*
     * Create the segment.
     */
    if ((shmid = shmget(key, size, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    /*
     * Attach the segment to our data space.
     */
    if ((shm_pointer = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }
    memcpy(shm_pointer,&ndevs,sizeof(uint8_t));
}
char *shm_pointer;
uint8_t ndevs;
uint8_t shm_connect(){
    int shmid;
    key_t key;
    key = 1339;
    if ((shmid = shmget(key, 1, 0666)) < 0) {
        perror("shmget");
        exit(1);
	}
    if ((shm_pointer = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }
		memcpy(&ndevs,shm_pointer,sizeof(uint8_t));
        uint16_t size = 2+(sizeof(MotorController)*ndevs);
        shmdt(shm_pointer);
        if ((shmid = shmget(key, size, 0666)) < 0) {
            perror("shmget");
            exit(1);
        }
		if ((shm_pointer = shmat(shmid, NULL, 0)) == (char *) -1) {
            perror("shmat");
            exit(1);
		}
        return ndevs;
    
}
void shm_srv_write(MotorController *m ,uint8_t ndevs){
	MotorController *temp;
	int i=0;
	for(i=0; i<ndevs; i++){
	temp=(MotorController *)(shm_pointer+2+(i*sizeof(MotorController)));
	// memcpy all data except for last field
	memcpy(temp,&m[i],sizeof(MotorController)-sizeof(int16_t));
}
}
void shm_srv_read(MotorController *m ,uint8_t ndevs){
	MotorController *temp;
	int i=0;
	for(i=0; i<ndevs; i++){
	temp=(MotorController *)(shm_pointer+2+(i*sizeof(MotorController))-sizeof(int16_t));
	// memcpy last field only
	memcpy(&m[i].dout_Vout,temp,sizeof(int16_t));
	}
}
