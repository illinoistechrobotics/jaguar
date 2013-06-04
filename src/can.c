#include<stdint.h>
char data[4];
int len=0;
uint32_t CreateMessageID(uint8_t canid, uint8_t apicls, uint8_t apinum, uint8_t mfgid, uint8_t devtype)
{
uint32_t id;
id = canid & 0x3f;
id |= ((apinum & 0x0f)<<6);
id |= ((apicls & 0x7f)<<10);
id |= mfgid << 16;
id |= devtype << 24;
return id;
}
float fixed2float(int whole, int frac){
float out = (float)whole+((float)frac/256.0);
return out;
}
void SetDataSignedInt16(int16_t in){
data[0] = in & 0xff;
data[1] = (in>>8) & 0xff;
len=2;
}
void SetDataSignedInt32(int32_t in){
data[0] = in & 0xff;
data[1] = (in>>8) & 0xff;
data[2] = (in>>16) & 0xff;
data[3] = (in>>24) & 0xff;
len=4;
}
void ClearData(){
len=0;
}
int CANSendMessage(uint32_t ulID)
{
   int i;
   if(serial_init==1){
      uint8_t msg[6+len];
      //Start of packet indicator.  
      msg[0]=0xff;

      //
      // Set the length of the data packet.
      //
      msg[1]=(uint8_t)(len + 4);

      //
      // Set the message ID.
      //
      msg[2]=(ulID & 0xff);
      msg[3]=((ulID >> 8) & 0xff);
      msg[4]=((ulID >> 16) & 0xff);
      msg[5]=((ulID >> 24) & 0xff);

      //
      // Set the associated data
      //
      for(i=0; i<len; i++){
         msg[6+i]=data[i];
      }
      //
      // Send the constructed message.
      //
      const char esc1[] = {0xfe,0xfe};
      const char esc2[] = {0xfe,0xfd};
      	write(serial_file,&msg[0],1);
	printf("Sent: %x\r\n",(msg[0] & 0xff));
	int c;
	for(c=1;c<(6+len);c++){
	if(msg[c]==0xff){
	write(serial_file,&esc1,2);
	printf("Sent: 0xfe 0xfe\r\n");
	}
	else if(msg[c]==0xfe){
	write(serial_file,&esc2,2);
	printf("Sent: 0xfe 0xfd\r\n");
	} else {
	write(serial_file,&msg[c],1);
	printf("Sent: %x\r\n",(msg[c] & 0xff));
	}
 	}
         return 0;
      }
	else {
	return 1;
   }
}
