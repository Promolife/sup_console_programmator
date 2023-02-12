void memoryReset()
{
  LED_READ_ON();
  K5L_RESET();
  K5L_READ_INIT();
  LED_READ_OFF();
  sendOK();
}

void memoryType()                                                               // ManufacturedID и DeviceID
{
  LED_READ_ON();
  K5L_READ_FLASHID();
  K5L_RESET();
  for(unsigned char i=0; i < 7; i++)
  {
    USART0_Transmit(flashIDMessage[i]);
  }
  LED_READ_OFF();
  uart0_puts("\r\n");
}

void memoryErase()
{
  LED_READ_ON();
  LED_WRITE_ON();
  K5L_UNLOCK_ERASE();
  K5L_ERASE_FLASH();
  sendWAIT();
  K5L_READ_INIT();
  K5L_READ_ADDRESS_START();
  while((K5L_READ_WORD(0x00) == 0x4C) || (K5L_READ_WORD(0x00) == 0x08))      
  {
    delay_ms(10);
  }
  if(K5L_READ_WORD(0x00) != 0xffff)
  {
    sendERROR();
  }
  else
  {
    sendOK();
  }
  K5L_READ_ADDRESS_STOP();
  LED_READ_OFF();
  LED_WRITE_OFF();
}

unsigned char memoryWriteBlock(unsigned char *array)
{
  //---------------Переменные и определения-------
 unsigned int sizeBlock= 0;       
 unsigned int blockCRC16 = 0;
 unsigned int dataCRC = 0;
 unsigned int checkCRC = 0;
 unsigned long addressBlock = 0;
 unsigned int writeWord= 0;
 unsigned int tempWord = 0;
 
 unsigned char LowByte = 0;
 unsigned char HiByte = 0;
 unsigned char parityTick = 0;
 unsigned int parityCounter = 0;

 
//----------------Начало логики------------------- 
 LED_WRITE_OFF();
 LED_READ_OFF();
 sizeBlock = to_Bit16(array[4],array[5]);
 if(sizeBlock > RX0_Buffer_Size-8)
 {
   sendERROR();
   return 1;
 }
 
 addressBlock = to_Bit32(array[6],array[7],array[8],array[9]);
 
 for(unsigned int crcCounter = 0; crcCounter < sizeBlock; crcCounter++)
   {
    dataCRC = (dataCRC ^ array[10 + crcCounter]);
        for(unsigned char i=0; i < 8; i++)
        {
            if(dataCRC & 0x0001)
                dataCRC = ((dataCRC >> 1) ^ 0xA001);
            else
                dataCRC = (dataCRC >> 1);
            dataCRC &=0xFFFF;
        }
   }

 blockCRC16 = to_Bit16(array[9+sizeBlock+1],array[9+sizeBlock+2]);
 
 if(dataCRC != blockCRC16)                                                      
 {
   sendCRCERROR();
   return 1;
 }
 else
 {
   LED_WRITE_ON();
   if(sizeBlock % 2)
   {
     for(unsigned int writeCounter = 0; writeCounter < sizeBlock; writeCounter++)
        {
          if(!parityTick)
          {
            LowByte = array[10+writeCounter];
            parityTick++;
          }
          else
          {
            HiByte = array[10+writeCounter];
            writeWord = to_Bit16(HiByte,LowByte);
            K5L_WRITE_WORD(addressBlock+parityCounter,writeWord);
            parityTick = 0;
            parityCounter++;
          }
        }
     writeWord = to_Bit16(0xFF,LowByte);
     K5L_WRITE_WORD(addressBlock+parityCounter,writeWord);
     
   }
   else
   {
      for(unsigned int writeCounter = 0; writeCounter < sizeBlock/2; writeCounter++)
        {
          writeWord = to_Bit16(array[10+(writeCounter*2)+1],array[10+(writeCounter*2)]);
          K5L_WRITE_WORD(addressBlock+writeCounter,writeWord);
        }
  
   }
   LED_WRITE_OFF();
   LED_READ_ON();
  
 K5L_READ_INIT();
 K5L_READ_ADDRESS_START();
 LowByte = 0;
 HiByte = 0;
 parityTick = 0;
 parityCounter = 0;
 
   tempWord = K5L_READ_WORD(addressBlock);
   for(unsigned int writeCounter = 0; writeCounter < sizeBlock; writeCounter++)
        {
          if(!parityTick)
          {
            checkCRC = (checkCRC ^ Lo8(tempWord));
            for(unsigned char i=0; i < 8; i++)
              {
                if(checkCRC & 0x0001)
                    checkCRC = ((checkCRC >> 1) ^ 0xA001);
                else
                    checkCRC = (checkCRC >> 1);
                checkCRC &=0xFFFF;
              }
            parityTick++;
          }
          else
          {
            checkCRC = (checkCRC ^ Hi8(tempWord));
            for(unsigned char i=0; i < 8; i++)
              {
                if(checkCRC & 0x0001)
                    checkCRC = ((checkCRC >> 1) ^ 0xA001);
                else
                    checkCRC = (checkCRC >> 1);
                checkCRC &=0xFFFF;
              } 
            parityTick = 0;
            parityCounter++;
            tempWord = K5L_READ_WORD(parityCounter + addressBlock);
          }
        }
K5L_READ_ADDRESS_STOP();
LED_READ_OFF();
 if(checkCRC != dataCRC)
  {
    //sendOK();
    uart0_puts("$WRTCRCERR\r\n");
    return 1;
  }
  else
  {
    sendOK(); 
  } 
 }
 return 0;
}

void memoryReadRange(unsigned char *array)
{
  unsigned long addressBlockBegin = 0;
  unsigned long addressBlockEnd = 0;
  addressBlockBegin = to_Bit32(array[4],array[5],array[6],array[7]);
  addressBlockEnd = to_Bit32(array[8],array[9],array[10],array[11]);
  sendOK();
  delay_ms(1000);
  LED_READ_ON();
  K5L_READ_RANGE_FLASH(addressBlockBegin,addressBlockEnd);
  LED_READ_OFF();
}

void memoryReadAddress(unsigned char *array)
{
  LED_READ_ON();
  unsigned long addressBlockBegin = 0;
  addressBlockBegin = to_Bit32(array[4],array[5],array[6],array[7]);
  K5L_READ_RANGE_FLASH(addressBlockBegin,addressBlockBegin+1);
  LED_READ_OFF();
}