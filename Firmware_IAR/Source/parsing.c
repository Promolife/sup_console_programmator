void endParsingOperation()                                                      // End parsing operation
{
  RX0_Buffer_Clear();                                                           // Clear RX buffer
  USART0_RECIEVED_ENABLE();                                                     // Enable UART
  RX_Status = 0x01;                                                             // Flag possible RX operation
}

void parsingSort(unsigned char *array)                                          // Sorting data in buffer
{
  if(array[0] != '$')                                                           // Check start byte
  {
    sendERROR();                                                                // Send error
    endParsingOperation();                                                      // end parsing
  }
  
  else                                                                          // Hmm, maybe is programmator data?
  {
    if(array[1] == 'A' && array[2] == 'U' && array[3] == 'T')                   // Found $AUT (Are you there?)
    {
      sendOK();                                                                 // Say Œ 
      endParsingOperation();
    }
    else if(array[1] == 'M' && array[2] == 'R' && array[3] == 'T')              // Found $MRT (Memory Type)
    {
       memoryType();
       endParsingOperation();
    }
    else if(array[1] == 'E' && array[2] == 'R' && array[3] == 'S')              // Found $ERS (Erase)
    {
       memoryErase();
       endParsingOperation();
    }
    else if(array[1] == 'W' && array[2] == 'R' && array[3] == 'B')              // Found $WRB (Write block)
    {
       memoryWriteBlock(array);
       endParsingOperation();
    }
    else if(array[1] == 'R' && array[2] == 'D' && array[3] == 'R')              // Found $RDD (Read Range)
    {
       memoryReadRange(array);
       endParsingOperation();
    }
    else if(array[1] == 'R' && array[2] == 'S' && array[3] == 'T')              // Found $RST (Reset)
    {
       memoryReset();
       endParsingOperation();
    }
    else if(array[1] == 'R' && array[2] == 'D' && array[3] == 'A')              // Found $RDA (Read Address)
    {
       memoryReadAddress(array);
       endParsingOperation();
    }
    
       
    else                                                                        // Command is't found        
    {
      uart0_puts("$NODATA");                                                    // Say about
      endParsingOperation();                                                    
    }
  }
}