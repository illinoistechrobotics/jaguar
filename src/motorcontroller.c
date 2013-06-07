int InitMotorController(uint8_t mode,MotorController *mot){
CANMessage cm;
CANClearData(&cm);
CANMessageSetIDDefaults(mot->canid,&mot);
cm.apicls=mode;
cm.apinum=0;
CANSendMessage(&cm);
tcdrain(serial_file);
return 0;
void MotorControllerPrintf(MotorController *mot){
printf("------ Motor Controller ID: %d ------\n",mot->canid);
printf("Input Voltage %f V\n",mot->din_Vbus);
printf("Output Voltage %f V\n",mot->din_Vout);
printf("Output Current %f A\n",mot->din_Ibus);
printf("Temperature %f degC\n",mot->din_temp);
printf("Over Current %d\n",(mot->din_fault)&0x01);
printf("Over Temperature %d\n",(mot->din_fault>>1)&0x01);
printf("Input Voltage Fault %d\n",(mot->din_fault>>2)&0x01);
printf("------End Motor Controller------");
}
int ReadMotorController(MotorController *mot){
CANMessage cansend,canrecv;
tcdrain(serial_file);
//Prepare message;
CANClearData(canrecv);
CANMessageSetIDDefaults(mot->canid,&cansend);
cansend.apicls=5;
cansend.apinum=1; //Vbus
CANSendMessage(&cansend);
//Wait for message to send + 5 ms for response to arrive
tcdrain(serial_file);
usleep(5000);
// Should get 2 byts of data back
if(CANRecvMessage(&canrecv)!=0 || canrecv->datalen != 2){
	return -1;
}
mot->din_Vbus=fixed2float(canrecv.data[0],canrecv.data[1]);
//Re-init values;
CANClearData(canrecv);
cansend.apicls=5;
cansend.apinum=2; //Ibus
CANSendMessage(&cansend);
//Wait for message to send + 5 ms for response to arrive
tcdrain(serial_file);
usleep(5000);
if(CANRecvMessage(&canrecv)!=0 || canrecv->datalen != 2){
        return -1;
}
mot->din_Ibus=fixed2float(canrecv.data[0],canrecv.data[1]);
//Re-init values;
CANClearData(canrecv);
cansend.apicls=5;
cansend.apinum=10; //Vout
CANSendMessage(&cansend);
//Wait for message to send + 5 ms for response to arrive
tcdrain(serial_file);
usleep(5000);
if(CANRecvMessage(&canrecv)!=0 || canrecv->datalen != 2){
        return -1;
}
mot->din_Vout=fixed2float(canrecv.data[0],canrecv.data[1]);
//Re-init values;
CANClearData(canrecv);
cansend.apicls=5;
cansend.apinum=3; //Temperature
CANSendMessage(&cansend);
//Wait for message to send + 5 ms for response to arrive
tcdrain(serial_file);
usleep(5000);
if(CANRecvMessage(&canrecv)!=0 || canrecv->datalen != 2){
        return -1;
}
mot->din_temp=fixed2float(canrecv.data[0],canrecv.data[1]);
//Re-init values;
CANClearData(canrecv);
cansend.apicls=5;
cansend.apinum=7; //Faults
CANSendMessage(&cansend);
//Wait for message to send + 5 ms for response to arrive
tcdrain(serial_file);
usleep(5000);
if(CANRecvMessage(&canrecv)!=0 || canrecv->datalen != 1){
        return -1;
}
mot->din_fault=canrecv.data[0];
return 0;
}
int WriteMotorController(MotorController *mot){
CANMessage cansendw;
CANMessageSetIDDefaults(mot->canid,&cansendw);
memcpy(cansendw.data,&mot->dout_Vout,4);
cansendw.datalen = 4;
cansendw.apicls = 0;
cansendw.apinum = 2;
CANSendMessage(cansendw);

return 0;
}
