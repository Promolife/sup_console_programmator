/*K5L2731CCA-D770 Flash chip driver
Author: Aleksandr A. Serdykov, Russia, Krasnodar 
Email: promolife@list.ru
*/

//---------------------SetUp for Flash--------
#define MByte_address_ddr   DDRL
#define MLByte_address_ddr  DDRC
#define LByte_address_ddr   DDRA

#define MByte_address_port  PORTL
#define MLByte_address_port PORTC
#define LByte_address_port  PORTA

#define MByte_data_ddr      DDRK
#define LByte_data_ddr      DDRF

#define MByte_data_pin     PINK
#define LByte_data_pin     PINF
#define MByte_data_port    PORTK
#define LByte_data_port    PORTF

#define Control_ddr         DDRD

#define Control_port        PORTD
#define Control_port_CE     0
#define Control_port_OE     1
#define Control_port_WE     2
#define Control_port_WP     3

#define Flash_address_begin 0x000000
#define Flash_address_end   0x7FFFFF

unsigned char flashIDMessage[7];

//------------Function of Flash----------------------
void K5L_SET_ADDRESS(unsigned long address)
{
    LByte_address_port  =  ((address) & 0x000000FF);
    MLByte_address_port =  ((address >> 8) & 0x000000FF);
    MByte_address_port  =  ((address >> 16) & 0x000000FF);
}

void K5L_SET_DATA(unsigned int data)
{
   MByte_data_port = Hi8(data);
   LByte_data_port = Lo8(data);
}

void K5L_READ_INIT()
{
  MByte_address_ddr = 0xFF;
  MLByte_address_ddr = 0xFF;
  LByte_address_ddr = 0xFF;
  Control_ddr = 0xFF;
  MByte_data_ddr = 0x00;
  LByte_data_ddr = 0x00;
  SetBit(Control_port,Control_port_WP);
  K5L_SET_ADDRESS(0x00);
  K5L_SET_DATA(0x00);
}

void K5L_READ_ADDRESS_START()
{
  ClrBit(Control_port,Control_port_CE);
  ClrBit(Control_port,Control_port_OE);
  SetBit(Control_port,Control_port_WE);
}

void K5L_READ_ADDRESS_STOP()
{
  SetBit(Control_port,Control_port_CE);
  SetBit(Control_port,Control_port_OE);
  SetBit(Control_port,Control_port_WE);
}

unsigned int K5L_READ_WORD(unsigned long address)
{
  unsigned int data = 0;
  K5L_SET_ADDRESS(address);
  data = (MByte_data_pin << 8);
  data |= data | LByte_data_pin;
  return data;  
}


void K5L_READ_RANGE_FLASH(unsigned long bgn_add,unsigned long end_add)
{
  unsigned int data = 0;
  K5L_READ_INIT();
  K5L_READ_ADDRESS_START();
  for(unsigned long read_counter = bgn_add; read_counter < end_add; read_counter++)
  {
    data = K5L_READ_WORD(read_counter);
    USART0_Transmit(Lo8(data));
    USART0_Transmit(Hi8(data));
    delay_us(50);  // Need for hi-speed
  }
  K5L_READ_ADDRESS_STOP();
}


//---------------------------Clear, Read and Write Flash--------------
void K5L_WRITE_INIT()
{
  MByte_address_ddr = 0xFF;
  MLByte_address_ddr = 0xFF;
  LByte_address_ddr = 0xFF;
  Control_ddr = 0xFF;
  MByte_data_ddr = 0xFF;
  LByte_data_ddr = 0xFF;
  SetBit(Control_port,Control_port_WP);
  K5L_SET_ADDRESS(0xFFFFFFFF);
  K5L_SET_DATA(0xFFFF);
}

void K5L_RESET()
{
  K5L_WRITE_INIT();
  SetBit(Control_port,Control_port_OE);
  ClrBit(Control_port,Control_port_CE);
  ClrBit(Control_port,Control_port_WE);
  K5L_SET_DATA(0xF0);
  SetBit(Control_port,Control_port_WE);
  K5L_SET_ADDRESS(0x555);
  ClrBit(Control_port,Control_port_WE);
  K5L_SET_DATA(0xF0);
  SetBit(Control_port,Control_port_WE);
  SetBit(Control_port,Control_port_CE);
}

void K5L_ERASE_FLASH()
{
  K5L_WRITE_INIT();
  K5L_SET_ADDRESS(0x555);
  ClrBit(Control_port,Control_port_CE);
  SetBit(Control_port,Control_port_OE);
  ClrBit(Control_port,Control_port_WE);
  K5L_SET_DATA(0xAA);
  
  SetBit(Control_port,Control_port_CE);
  SetBit(Control_port,Control_port_WE);
  K5L_SET_ADDRESS(0x2AA);
  ClrBit(Control_port,Control_port_CE);
  ClrBit(Control_port,Control_port_WE);
  K5L_SET_DATA(0x55);
  
  SetBit(Control_port,Control_port_CE);
  SetBit(Control_port,Control_port_WE);
  K5L_SET_ADDRESS(0x555);
  ClrBit(Control_port,Control_port_CE);
  ClrBit(Control_port,Control_port_WE);
  K5L_SET_DATA(0x80);
  
  SetBit(Control_port,Control_port_CE);
  SetBit(Control_port,Control_port_WE);
  K5L_SET_ADDRESS(0x555);
  ClrBit(Control_port,Control_port_CE);
  ClrBit(Control_port,Control_port_WE);
  K5L_SET_DATA(0xAA);  
  
  SetBit(Control_port,Control_port_CE);
  SetBit(Control_port,Control_port_WE);
  K5L_SET_ADDRESS(0x2AA);
  ClrBit(Control_port,Control_port_CE);
  ClrBit(Control_port,Control_port_WE);
  K5L_SET_DATA(0x55);
  
  SetBit(Control_port,Control_port_CE);
  SetBit(Control_port,Control_port_WE);
  K5L_SET_ADDRESS(0x555);
  ClrBit(Control_port,Control_port_CE);
  ClrBit(Control_port,Control_port_WE);
  K5L_SET_DATA(0x10);
  
  SetBit(Control_port,Control_port_WE);
  SetBit(Control_port,Control_port_CE); 
}

void K5L_READ_FLASHID()
{
  K5L_WRITE_INIT();
  K5L_SET_ADDRESS(0x555);
  ClrBit(Control_port,Control_port_CE);
  SetBit(Control_port,Control_port_OE);
  ClrBit(Control_port,Control_port_WE);
  K5L_SET_DATA(0xAA);
  
  SetBit(Control_port,Control_port_CE);
  SetBit(Control_port,Control_port_WE);
  K5L_SET_ADDRESS(0x2AA);
  ClrBit(Control_port,Control_port_CE);
  ClrBit(Control_port,Control_port_WE);
  K5L_SET_DATA(0x55);
  
  SetBit(Control_port,Control_port_WE);
  K5L_SET_ADDRESS(0x555);
  ClrBit(Control_port,Control_port_WE);
  K5L_SET_DATA(0x90);
  SetBit(Control_port,Control_port_WE);
  
  K5L_READ_INIT();
  K5L_READ_ADDRESS_START();
  flashIDMessage[0] = Lo8(K5L_READ_WORD(0x00));
  flashIDMessage[1] = Lo8(K5L_READ_WORD(0x01));
  flashIDMessage[2] = Hi8(K5L_READ_WORD(0x01));
  flashIDMessage[3] = Lo8(K5L_READ_WORD(0x0E));
  flashIDMessage[4] = Hi8(K5L_READ_WORD(0x0E));
  flashIDMessage[5] = Lo8(K5L_READ_WORD(0x0F));
  flashIDMessage[6] = Hi8(K5L_READ_WORD(0x0F));
  K5L_READ_ADDRESS_STOP();  
}

void K5L_WRITE_WORD(unsigned long address, unsigned int data)
{
  K5L_WRITE_INIT();
  
  SetBit(Control_port,Control_port_CE);
  ClrBit(Control_port,Control_port_OE);
  SetBit(Control_port,Control_port_WE);
  
  K5L_SET_ADDRESS(0x555);
  K5L_SET_DATA(0xAA);
  ClrBit(Control_port,Control_port_CE);
  SetBit(Control_port,Control_port_OE);
  ClrBit(Control_port,Control_port_WE);
    
  SetBit(Control_port,Control_port_CE);
  SetBit(Control_port,Control_port_WE);
  K5L_SET_ADDRESS(0x2AA);
  K5L_SET_DATA(0x55);
  
  ClrBit(Control_port,Control_port_CE);
  ClrBit(Control_port,Control_port_WE);
  SetBit(Control_port,Control_port_WE);
  K5L_SET_ADDRESS(0x555);
  K5L_SET_DATA(0xA0);
    
  ClrBit(Control_port,Control_port_WE);
  SetBit(Control_port,Control_port_WE);
  K5L_SET_ADDRESS(address);
  K5L_SET_DATA(data);                                                           
  ClrBit(Control_port,Control_port_WE);
  SetBit(Control_port,Control_port_WE);
}

void K5L_UNLOCK_ERASE()
{
  K5L_WRITE_INIT();
  SetBit(Control_port,Control_port_OE);
  ClrBit(Control_port,Control_port_CE);
  ClrBit(Control_port,Control_port_WE);
  K5L_SET_DATA(0x80);
  SetBit(Control_port,Control_port_WE);
  K5L_SET_ADDRESS(0x555);
  ClrBit(Control_port,Control_port_WE);
  K5L_SET_DATA(0x10);
  SetBit(Control_port,Control_port_WE);
  SetBit(Control_port,Control_port_CE);
}

