//-----------Define tact frequency---------
#define Fosc 16000000                                                           // CPU frequency in Hz
#define Fosc_Hz  Fosc
#define Fosc_kHz (Fosc/1000)
#define Fosc_MHz (Fosc_kHz/1000)

//------------Define CPU time delay-------------------
#define delay_us(x) __delay_cycles(x*Fosc_MHz)
#define delay_ms(x) __delay_cycles(x*Fosc_kHz)
#define delay_s(x) __delay_cycles(x*Fosc_Hz)

//------------------ Util-------------------
#define SetBit(address,bit) (address|=(1<<bit))
#define ClrBit(address,bit) (address&=~(1<<bit))
#define InvBit(address,bit) (address^=(1<<bit))
#define SetFlag(byte,flag)  (byte|=flag)
#define ClrFlag(byte,flag)  (byte&=~flag)
//------------------------------------------
#define Hi8(var) (unsigned int) (var >> 8) & 0x00FF
#define Lo8(var) (unsigned int) var & 0x00FF
//-------------------------------------------
unsigned int to_Bit16(
                      unsigned char Hi,
                      unsigned char Lo
                        )
{                      
    unsigned int tmp;  tmp = Hi << 8 ; tmp |= tmp | Lo; return tmp;
}
//------------------------------------------
unsigned long to_Bit32(
                       unsigned char Hi0, 
                       unsigned char Hi1, 
                       unsigned char Lo0, 
                       unsigned char Lo1
                         )
{
    unsigned int tmp1 = 0;
    unsigned long tmp2 = 0;
    tmp1 = to_Bit16(Lo0,Lo1);
    tmp2 = to_Bit16(Hi0,Hi1) * 0x10000;
    return tmp1 + tmp2;
    
}

//-----------------All global variables----------------------------

//---------Status and buttons flags
unsigned char Status_button = 0;
#define BUTTON_R_FLAG 0x01
#define BUTTON_E_FLAG 0x02
#define BUTTON_W_FLAG 0x04

/*----------------Statuses, flags and buffer size USART-----------*/
#define RX0_Buffer_Size 2048+16                                                 // Size buffer + service data
#define RX0_Timeout     1

unsigned char RX0_Buffer[RX0_Buffer_Size];                                      // Массив буфера 0
unsigned int RX0_Buffer_Counter=0;

unsigned char RX_Status = 0x00;                                                 // Status USART
#define RX_Enable       0x01                                                    // Flag of reciev enable
#define RX_Timer_Start  0x02                                                    // Flag of start timeout count
#define RX_Timeout      0x04                                                    // Flag of timeout
#define RX_Parsing      0x08                                                    // Flag of beginin parsing
#define RX_Overflow     0x10                                                    // Flag of overflow buffer

//--------------Timers-----------
unsigned int Timer0_Counter = 0;
unsigned char Timer0_UART_Timeout_Counter = 0;

//-----------Parsing-----------------
unsigned long add_wr = 0;

void INIT_IO()
{
SREG |= (1 << 7); // All Interrupt enable 
}
