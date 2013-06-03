#include<stdint.h>
uint32_t CreateMessageID(uint8_t canid, uint16_t api, uint8_t mfgid, uint8_t devtype)
{
uint32_t id;
id = canid & 0x3f;
id |= ((api & 0x03ff)<<6);
id |= mfgid << 16;
id |= devtype << 24;
return id;
}
int CANSendMessage(uint32_t ulID, const char* data, int len)
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
      if(write(serial_file,&msg,(6+len))==0){
         return 0;
      } else {
         return 1;
      }
   } else {
      return 1;
   }
}
