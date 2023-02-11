#define BUTT_DDR  DDRB
#define BUTT_PORT PORTB
#define BUTT_PIN  PINB
#define BUTT_R    6
#define BUTT_E    5
#define BUTT_W    4

#define BUTT_CHECK_DELAY 30

void BUTT_INIT()
{
  BUTT_DDR |= (0<<BUTT_R)|(0<<BUTT_E)|(0<<BUTT_W);
  SetBit(BUTT_PORT,BUTT_R);
  SetBit(BUTT_PORT,BUTT_E);
  SetBit(BUTT_PORT,BUTT_W);
}

void ButtonRead()
{
  Status_button = 0;
  if((BUTT_PIN & (1 << BUTT_R)) == 0)
  {
    delay_ms(BUTT_CHECK_DELAY);
    
    if((BUTT_PIN & (1 << BUTT_R)) == 0)
      Status_button |= BUTTON_R_FLAG;
  }
  if((BUTT_PIN & (1 << BUTT_E)) == 0)
  {
    delay_ms(BUTT_CHECK_DELAY);
    
    if((BUTT_PIN & (1 << BUTT_E)) == 0)
      Status_button |= BUTTON_E_FLAG;
  }
  if((BUTT_PIN & (1 << BUTT_W)) == 0)
  {
    delay_ms(BUTT_CHECK_DELAY);
    
    if((BUTT_PIN & (1 << BUTT_W)) == 0)
      Status_button |= BUTTON_W_FLAG;
  }
    
}