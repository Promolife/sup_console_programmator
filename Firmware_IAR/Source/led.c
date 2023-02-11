#define LED_DDR  DDRH
#define LED_PORT PORTH
#define LED_W    4
#define LED_R    3

void LED_INIT()
{
  LED_DDR |= (1<<LED_W)|(1<<LED_R);
  ClrBit(LED_PORT,LED_W);
  ClrBit(LED_PORT,LED_R);  
}

void LED_WRITE_ON()
{
  SetBit(LED_PORT,LED_W);
}

void LED_WRITE_OFF()
{
  ClrBit(LED_PORT,LED_W);
}

void LED_READ_ON()
{
  SetBit(LED_PORT,LED_R);
}

void LED_READ_OFF()
{
  ClrBit(LED_PORT,LED_R);
}