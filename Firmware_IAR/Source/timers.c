void TIMER0_INIT()                                                              // Timer 0 On for count of milliseconds
{
  OCR0A  = 125;
  TCCR0B |= (0 << CS02)| (1 << CS01) | (1 << CS00);
  TIMSK0 |= (1<<OCIE0A);
}

void UART0_Timeout()                                                            // Count RX timeout with Timer 0
{
    if(RX_Status & RX_Timer_Start)                                              
  {
    Timer0_UART_Timeout_Counter++;
    if(Timer0_UART_Timeout_Counter > RX0_Timeout)
    {
      USART0_RECIEVED_DISABLE();
      ClrFlag(RX_Status,RX_Enable);
      ClrFlag(RX_Status,RX_Timer_Start);
      SetFlag(RX_Status,RX_Timeout);
      SetFlag(RX_Status,RX_Parsing);
      Timer0_UART_Timeout_Counter = 0;
      RX0_Buffer_Counter = 0;
    }
  }
  
}

/*--------------------Interrupt Timer 0---------------------------------*/
#pragma vector = TIMER0_COMPA_vect
__interrupt void time_counter0()
{
  UART0_Timeout();
  Timer0_Counter++;
}
