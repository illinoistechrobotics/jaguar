#include <signal.h>
#define PRINTFORMAT "%f"
void CHKERR(int err){
#define nodp 600
	if(err!=0){
		printf("FIFO Creation Error!");
		exit(EXIT_FAILURE);
	}
}
void FIFOSetup(MotorController *m ,FIFOTree *ft){
	signal(SIGPIPE,SIG_IGN);
	int ierr;
	ft->canid=m->canid;
	char dirname[8];
	char fifopath[32];
	// First delete any levtover directories or FIFOS
	char rmcmd[32] = "rm -rf ";
	sprintf(dirname,"%d/",m->canid);
	strncpy(ft->dirname,dirname,8);
	strncat(rmcmd,dirname,24);
	system(rmcmd);
	ierr=mkdir(dirname,nodp);CHKERR(ierr);
	strncpy(fifopath,dirname,8);
	strncat(fifopath,"din_Vbus",22);
	ierr=mknod(fifopath,nodp,0);CHKERR(ierr);
	ft->din_Vbus_f=open(fifopath,O_WRONLY|O_NONBLOCK);
	strncpy(fifopath,dirname,8);
	strncat(fifopath,"din_Vout",22);
	ierr=mknod(fifopath,nodp,0);CHKERR(ierr);
	ft->din_Vout_f=open(fifopath,O_WRONLY|O_NONBLOCK);
	strncpy(fifopath,dirname,8);
	strncat(fifopath,"din_Ibus",22);
	ierr=mknod(fifopath,nodp,0);CHKERR(ierr);
	ft->din_Ibus_f=open(fifopath,O_WRONLY|O_NONBLOCK);
	strncpy(fifopath,dirname,8);
	strncat(fifopath,"din_temp",22);
	ierr=mknod(fifopath,nodp,0);CHKERR(ierr);
	ft->din_temp_f=open(fifopath,O_WRONLY|O_NONBLOCK);
	strncpy(fifopath,dirname,8);
	strncat(fifopath,"din_fault",22);
	ierr=mknod(fifopath,nodp,0);CHKERR(ierr);
	ft->din_fault_f=open(fifopath,O_WRONLY|O_NONBLOCK);
}

int FIFOWrite(MotorController *m ,FIFOTree *ft){
	int stat;
	stat = dprintf(ft->din_Vbus_f,PRINTFORMAT,m->din_Vbus);
	stat += dprintf(ft->din_Vout_f,PRINTFORMAT,m->din_Vout);
	stat += dprintf(ft->din_Ibus_f,PRINTFORMAT,m->din_Ibus);
	stat += dprintf(ft->din_temp_f,PRINTFORMAT,m->din_temp);
	stat += dprintf(ft->din_fault_f,"%x",m->din_fault);
	return stat;
}
int FIFORead(MotorController *m ,FIFOTree *ft){
	int stat=0;
	int16_t power;
	char inbuf[32];
	char fifopath[32];
	strncpy(fifopath,ft->dirname,8);
        strncat(fifopath,"dout_Vout",22);
	ft->dout_Vout_f=open(fifopath,O_RDONLY|O_NONBLOCK);
	if(ft->dout_Vout_f != -1){
	if(read(ft->dout_Vout_f,inbuf,32)>0){
		printf("%s recieved from FIFO",inbuf);
		power = atoi(inbuf);
		m->dout_Vout=power;
		stat=0;
	}
}
	else{
		stat=1;
	}
	return stat;
}

