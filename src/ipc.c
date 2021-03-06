#include <sys/ipc.h>
#include <sys/shm.h>
uint8_t safety;
char *shm_pointer;
void shm_setup(uint8_t ndevsi){
    int shmid;
    key_t key;
    key = 1339;
	uint16_t size = 2+(sizeof(MotorController)*ndevsi);
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
    memcpy(shm_pointer,&ndevsi,sizeof(uint8_t));
}
char *shm_pointer;
uint8_t shm_connect(){
    int shmid;
    uint8_t devs;
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
		memcpy(&devs,shm_pointer,sizeof(uint8_t));
        uint16_t size = 2+(sizeof(MotorController)*devs);
        shmdt(shm_pointer);
        if ((shmid = shmget(key, size, 0666)) < 0) {
            perror("shmget");
            exit(1);
        }
		if ((shm_pointer = shmat(shmid, NULL, 0)) == (char *) -1) {
            perror("shmat");
            exit(1);
		}
        return devs;
    
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
	temp=(MotorController *)(shm_pointer+2);
	for(i=0; i<ndevs; i++){

	// get last field only
	m[i].dout_Vout=temp[i].dout_Vout;
	}
}
void shm_cli_write(MotorController *m ,uint8_t ndevs){
	MotorController *temp;
	int i=0;
	temp=(MotorController *)(shm_pointer+2);
	for(i=0; i<ndevs; i++){

	// set last field only
	temp[i].dout_Vout=m[i].dout_Vout;
	}
}
void shm_cli_read(MotorController *m ,uint8_t ndevs){
        MotorController *temp;
        int i=0;
        for(i=0; i<ndevs; i++){
        temp=(MotorController *)(shm_pointer+2+(i*sizeof(MotorController)));
        // memcpy all data except for last field
        memcpy(&m[i],temp,sizeof(MotorController)-sizeof(int16_t));
}       
}       

