#define RWDELAY 500
int InitMotorController(uint8_t mode,MotorController *mot){
CANMessage cm;
CANClearData(&cm);
CANMessageSetIDDefaults(mot->canid,&cm);
cm.apicls=mode;
cm.apinum=0;
CANSendMessage(&cm);
tcdrain(serial_file);
return 0;
}
void MotorControllerPrintf(MotorController *mot){
printf("------ Motor Controller ID: %d ------\n",mot->canid);
printf("Input Voltage %f V\n",mot->din_Vbus);
printf("Output Voltage %f V\n",mot->din_Vout);
printf("Output Current %f A\n",mot->din_Ibus);
printf("Temperature %f degC\n",mot->din_temp);
printf("Over Current %d\n",(mot->din_fault)&0x01);
printf("Over Temperature %d\n",(mot->din_fault>>1)&0x01);
printf("Input Voltage Fault %d\n",(mot->din_fault>>2)&0x01);
printf("------End Motor Controller------\n\n");
}
int ReadMotorController(MotorController *mot){
// WARNING: CAN Messages may be out of order!!

CANMessage cansend,canrecv;
int rval;
tcdrain(serial_file);
//Prepare message;
CANClearData(&canrecv);
CANMessageSetIDDefaults(mot->canid,&cansend);
cansend.apicls=5;
cansend.apinum=1; //Vbus
CANSendMessage(&cansend);
//Wait for message to send + 5 ms for response to arrive
tcdrain(serial_file);
usleep(RWDELAY);
// Should get 2 bytes of data back
rval = 0; 
while(rval==0 && (canrecv.apicls!=5 || canrecv.apinum != 1)){
rval=CANRecvMessage(&canrecv);
}
if(canrecv.datalen != 2){
	printf("Error reading Vbus: %d bytes recieved, 2 expected\n",canrecv.datalen);
	CANPrintf(&canrecv);
	return -1;
}
#ifdef DEBUG
printf("Data Byte 1 %x", canrecv.data[0]); printf("2: %x\n",canrecv.data[1]);
#endif
mot->din_Vbus=fixed2float(canrecv.data[1],canrecv.data[0]);
//Re-init values;
CANClearData(&canrecv);
cansend.apicls=5;
cansend.apinum=2; //Ibus
CANSendMessage(&cansend);
//Wait for message to send + 5 ms for response to arrive
tcdrain(serial_file);
usleep(RWDELAY);
rval = 0; 
while(rval == 0 && (canrecv.apinum != 2 || canrecv.apicls != 5)){
rval=CANRecvMessage(&canrecv);
}
if(canrecv.datalen != 2){
        printf("Error reading Ibus: %d bytes recieved, 2 expected\n",canrecv.datalen);
        CANPrintf(&canrecv);
        return -1;

}
mot->din_Ibus=fixed2float(canrecv.data[1],canrecv.data[0]);
//Re-init values;
CANClearData(&canrecv);
cansend.apicls=5;
cansend.apinum=10; //Vout
CANSendMessage(&cansend);
//Wait for message to send + 5 ms for response to arrive
tcdrain(serial_file);
usleep(RWDELAY);
rval = 0; 
while(rval==0 && (canrecv.apicls!=5 || canrecv.apinum != 10)){
rval=CANRecvMessage(&canrecv);
}
if(canrecv.datalen != 2){
        printf("Error reading Vout: %d bytes recieved, 2 expected\n",canrecv.datalen);
        CANPrintf(&canrecv);
        return -1;
}
#ifdef DEBUG
printf("Data Byte 1 %x", canrecv.data[0]); printf("2: %x\n",canrecv.data[1]);
#endif
mot->din_Vout=fixed2float(abs((int8_t)canrecv.data[1]),canrecv.data[0]);
//Re-init values;
CANClearData(&canrecv);
cansend.apicls=5;
cansend.apinum=3; //Temperature
CANSendMessage(&cansend);
//Wait for message to send + 5 ms for response to arrive
tcdrain(serial_file);
usleep(RWDELAY);
rval = 0; 
while(rval==0 && (canrecv.apicls!=5 || canrecv.apinum != 3)){
rval=CANRecvMessage(&canrecv);
}
if(canrecv.datalen != 2){
        printf("Error reading temperarure: %d bytes recieved, 2 expected\n",canrecv.datalen);
        CANPrintf(&canrecv);
        return -1;
}
#ifdef DEBUG               
printf("Data Byte 1 %x", canrecv.data[0]); printf("2: %x\n",canrecv.data[1]);
#endif
mot->din_temp=fixed2float(canrecv.data[1],canrecv.data[0]);
//Re-init values;
CANClearData(&canrecv);
cansend.apicls=5;
cansend.apinum=7; //Faults
CANSendMessage(&cansend);
//Wait for message to send + 5 ms for response to arrive
tcdrain(serial_file);
usleep(RWDELAY);
rval = 0; 
while(rval==0 && (canrecv.apicls!=5 || canrecv.apinum != 7)){
rval=CANRecvMessage(&canrecv);
}
if(canrecv.datalen != 2){
        printf("Error reading faults: %d bytes recieved, 2 expected\n",canrecv.datalen);
        CANPrintf(&canrecv);
        return -1;
}
#ifdef DEBUG
printf("Data Byte 1 %x", canrecv.data[0]); printf("2: %x\n",canrecv.data[1]);       
#endif
mot->din_fault=canrecv.data[0];
return 0;
}
int WriteMotorController(MotorController *mot){
CANMessage cansendw;
CANMessageSetIDDefaults(mot->canid,&cansendw);
memcpy(cansendw.data,&mot->dout_Vout,4);
cansendw.datalen = 2;
cansendw.apicls = 0;
cansendw.apinum = 2;
CANSendMessage(&cansendw);

return 0;
}
