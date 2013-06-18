uint32_t CreateMessageID(uint8_t canid, uint8_t apicls, uint8_t apinum, uint8_t mfgid, uint8_t devtype){
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
void float2fixed88(int8_t *ptr, float in){
//Whole part
ptr[0]=(int8_t)in;
//Drop whole part and sign 
in=in-(int8_t)in;
//Calculate fractional part
ptr[1]=(in*256);
}
void CANSetDataSignedInt16(int16_t in, CANMessage *cmsg){
cmsg->data[0] = in & 0xff;
cmsg->data[1] = (in>>8) & 0xff;
cmsg->datalen=2;
}
void CANSetDataSignedInt32(int32_t in, CANMessage *cmsg){
cmsg->data[0] = in & 0xff;
cmsg->data[1] = (in>>8) & 0xff;
cmsg->data[2] = (in>>16) & 0xff;
cmsg->data[3] = (in>>24) & 0xff;
cmsg->datalen=4;
}
void CANClearData(CANMessage *cmsg){
cmsg->datalen=0;
}
void CANMessageSetIDDefaults(uint8_t id, CANMessage *cmsg){
cmsg->datalen=0;
cmsg->canid=id;
cmsg->mfgid=MFG_TI;
cmsg->devid=DEVTYPE_MOTORCNTL;
cmsg->apicls=5;
cmsg->apinum=0;
}
void CANPrintf(CANMessage *cmsg){
printf("------BEGIN CAN MESSAGE------\n");
printf("CAN ID: %d\n",cmsg->canid);
printf("API Class: %d\n",cmsg->apicls);
printf("API Number: %d\n",cmsg->apinum);
printf("Device ID: %d\n",cmsg->devid);
printf("Manufacturer ID: %d\n",cmsg->devid);
printf("Length of data: %d\n",cmsg->datalen);
int i;
for(i=0;i<(cmsg->datalen);i++){
	printf("Data Byte: %x\n",(unsigned int)(cmsg->data[i]));
}
printf("------END CAN MESSAGE------\n\n");
}

int CANRecvMessage(CANMessage *cmsg){
uint8_t bufraw[18];
int len,rawcount=2,unesccount=0;
uint8_t bufunesc[16];
if(serial_init==1){
	len=read(serial_file,bufraw,2);

	if(bufraw[0]!=0xff){
                //Invalid Message!
                return -1;
        }	
	len += read(serial_file,bufraw+2,bufraw[1]);
	#ifdef DEBUG
	printf("Message of length %d recieved\n",len);
	printf("CAN Device specified size: %d\n",bufraw[1]);
	#endif
	if(len<5 || len>12){
		//Invalid Message!
		return -2;
	}
	// Unescape ID + Data
	for(unesccount=0;unesccount<len-2;unesccount++){
		#ifdef DEBUG
		printf("rawcount %d\n",rawcount);
		printf("unesccount %d\n",unesccount);
		#endif
		if(unesccount<(len-3)){
		// Gen. case
			if((bufraw[rawcount]==0xfe)&&(bufraw[rawcount+1]==0xfe)){
				//This = 0xff
				bufunesc[unesccount]=0xff;
				rawcount += 2;
			
			} else if((bufraw[rawcount]==0xfe)&&(bufraw[rawcount+1]==0xfd)){
                                //This = 0xfe
                                bufunesc[unesccount]=0xfe;
                                rawcount += 2;
                       
                        } else {
				bufunesc[unesccount]=bufraw[rawcount];
		
				rawcount++;
			}
		}
		else{
                //Last Byte
                bufunesc[unesccount]=bufraw[rawcount];
                }
	#ifdef DEBUG
	printf("Recieved data: %x\n",bufunesc[unesccount]);
	#endif 	
	}
	if((uint8_t)bufraw[1]!=unesccount){
                //Invalid Length
                return -2;
        }
	#ifdef DEBUG
	printf("%d bytes unescaped\n",unesccount);
	printf("ID Bytes: %x ",bufunesc[0]);
	printf("%x ",bufunesc[1]);
	printf("%x ",bufunesc[2]);
	printf("%x\n",bufunesc[3]);
	#endif
	//Reverse-parse ID, bytes 3-6
	cmsg->canid = bufunesc[0] & 0x3f;
	cmsg->apinum = (bufunesc[0]>>6) & 0x03;
	cmsg->apinum |= (bufunesc[1]<<2) & 0x0c;
	cmsg->apicls = (bufunesc[1]>>2) & 0x3f;
	cmsg->mfgid = bufunesc[2];
	cmsg->devid = bufunesc[3] & 0x1f;
	cmsg->datalen=unesccount-4;
	//Memcpy data if necessary
	if(unesccount>4){
	//printf("%d bytes memcpy'ed", unesccount-4);
	memcpy(cmsg->data,&bufunesc[4],unesccount-4);
	}
	return 0;
}
else{
return -1;
}
}
int CANSendMessage(CANMessage *cmsg)
{
   int i;
   uint32_t ulID;
   ulID=CreateMessageID(cmsg->canid,cmsg->apicls,cmsg->apinum,cmsg->mfgid,cmsg->devid);
   if(serial_init==1){
	tcflush(serial_file, TCIFLUSH);
      uint8_t msg[6+(cmsg->datalen)];
      //Start of packet indicator.  
      msg[0]=0xff;

      //
      // Set the length of the data packet.
      //
      msg[1]=(uint8_t)(cmsg->datalen + 4);

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
      for(i=0; i<(cmsg->datalen); i++){
         msg[6+i]=(cmsg->data[i]);
      }
      //
      // Send the constructed message.
      //
      const char esc1[] = {0xfe,0xfe};
      const char esc2[] = {0xfe,0xfd};
      	write(serial_file,&msg[0],1);
	#ifdef DEBUG
	printf("Sent: %x\r\n",(msg[0] & 0xff));
	#endif
	int c;
	for(c=1;c<(6+(cmsg->datalen));c++){
	if(msg[c]==0xff){
	write(serial_file,&esc1,2);
	#ifdef DEBUG
	printf("Sent: 0xfe 0xfe\r\n");
	#endif
	}
	else if(msg[c]==0xfe){
	write(serial_file,&esc2,2);
	#ifdef DEBUG
	printf("Sent: 0xfe 0xfd\r\n");
	#endif
	} else {
	write(serial_file,&msg[c],1);
	#ifdef DEBUG
	printf("Sent: %x\r\n",(msg[c] & 0xff));
	#endif
	}
 	}
         return 0;
      }
	else {
	return 1;
   }
}
void CANBroadcastHeartbeat(){
CANMessage hb;
hb.canid=0;// Broadcast
hb.mfgid=0;
hb.devid=0; //System control message
hb.datalen=0;
hb.apicls=0;
hb.apinum=5;
CANSendMessage(&hb);
}
