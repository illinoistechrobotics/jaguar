int UpdateMotorController(MotorController *mot){
CANMessage cansend,canrecv;
char buf[20];
// Flush read buffer
tcflush(serial_file, TCIFLUSH);
// Begin reading values
CANMessageSetIDDefaults(mot->canid,&cansend);
//Vbus is api class 5 field 2
cansend->apicls=API_STATUS;
cansend->apinum=2;
CANSendMessage();
//Wait for message to send + 5 ms for response to arrive
tcdrain(serial_file);
usleep(5000);
