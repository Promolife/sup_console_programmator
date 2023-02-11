#define BAUD_0 1000000
#define UBRR_0 Fosc/8/BAUD_0-1


/*----------------UART Function-----------*/
void RX0_Buffer_Clear()
{
  for (unsigned int a = 0; a < RX0_Buffer_Size-1; a++) 
  RX0_Buffer[a] = 0;
}

void USART0_INIT()
{  
UBRR0H = UBRR_0>>8;
UBRR0L = UBRR_0;
UCSR0A |= (1 << U2X0);
UCSR0B |= (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0);
UCSR0C |= (0<<UMSEL00)|(0<<USBS0)|(1 << UCSZ01) | (1 << UCSZ00);
SetFlag(RX_Status,RX_Enable);                                                   
RX0_Buffer_Clear();
}

void USART0_Transmit( unsigned char data )
{
while ( !( UCSR0A & (1<<UDRE0)) );
UDR0 = data;
}

void USART0_RECIEVED_ENABLE()
{
  SetBit(UCSR0B,RXCIE0);
  SetFlag(RX_Status,RX_Enable);
}

void USART0_RECIEVED_DISABLE()
{
  ClrBit(UCSR0B,RXCIE0);
  ClrFlag(RX_Status,RX_Enable);
}


void uart0_puts(unsigned char *str)
{
unsigned char k;
while (k=*str++) 
    USART0_Transmit(k);
}

void sendOK()
{
  uart0_puts("$OK\r\n");
}

void sendERROR()
{
  uart0_puts("$ERR\r\n");
}

void sendCRCERROR()
{
  uart0_puts("$CRCERR\r\n");
}

void sendWAIT()
{
  uart0_puts("$WAIT\r\n");
}


#pragma vector = USART0_RX_vect
__interrupt void Recieve_0()
{
    RX0_Buffer[RX0_Buffer_Counter] = UDR0;                                      // Write recieved byte in buffer
    RX0_Buffer_Counter++;                                                       // Counter buffer +1
    if(RX0_Buffer_Counter == RX0_Buffer_Size)                                   // Counter buffer equals buffer size
      {
       RX0_Buffer_Counter = 0;                                                  // clear counter buffer
       USART0_RECIEVED_DISABLE();                                               // Turn off recieved to USART_0
       ClrFlag(RX_Status,RX_Enable);                                            // Clear recieve flag of USART_0, forbid received data
       SetFlag(RX_Status,RX_Overflow);                                          // Enable parsing , выставл€€ флаг парсинга в 1
       SetFlag(RX_Status,RX_Parsing);
      }
    Timer0_UART_Timeout_Counter = 0;                                            // Clear timeout counter
    SetFlag(RX_Status,RX_Timer_Start);                                          // Enable start flag of timeout timer
        
}
