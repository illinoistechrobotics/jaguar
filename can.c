int CANSendMessage(unsigned long ulID, const char* Data, int len)
{
    if(serial_init==1){
    int MsgLen = 0;
   char[6+len] msg
    //Start of packet indicator.  
    msg[0]=0xff;

    //
    // Set the length of the data packet.
    //
    msg[1]=(char)(len + 4);

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
    

    //
    // Send the constructed message.
    //
    
    
    return 0;
	} else {
	return 1;
	}
}