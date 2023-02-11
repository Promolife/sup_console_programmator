void main_init()                                                                // Main init
{
  INIT_IO();
  LED_INIT();
  BUTT_INIT();
  TIMER0_INIT();
  USART0_INIT();
  
  display_init();
  display_clear();
}

void main_start()
{
  
}

void main_loop()
{
  while(1)
  {
    ButtonRead();
    if(RX_Status & RX_Parsing)
      parsingSort(RX0_Buffer);  
    if(Status_button & BUTTON_R_FLAG)
    {
    }
    if(Status_button & BUTTON_W_FLAG)
    {
    }
    if(Status_button & BUTTON_E_FLAG)
    {
      memoryErase();
      endParsingOperation();
    }
  }
}