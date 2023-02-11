#define I2C_T            4000000 
#define F_CLK             Fosc

#define SDA_LN            PH1
#define SCL_LN            PH0
#define I2C_PIN           PINH
#define I2C_DDR           DDRH
#define I2C_PORT          PORTH

#define IN_LN             0
#define OUT_LN            1
#define ACK               1
#define NOT_ACK           0

unsigned char i2c_error;


unsigned char in_sda(void)
{
  if(I2C_PIN&(1<<SDA_LN))
    return 1;
  else
    return 0;
}

void sda_io(unsigned char io_c)
{
  if(io_c==IN_LN)
    I2C_DDR&=~(1<<SDA_LN);
  else
    I2C_DDR|=(1<<SDA_LN);
  __delay_cycles(F_CLK/I2C_T);
}


void scl_set(unsigned char set_c)
{
  if(set_c)
    {
      I2C_DDR&=~(1<<SCL_LN);
      I2C_PORT&=~(1<<SCL_LN);
    }
  else
    {
      I2C_DDR|=(1<<SCL_LN);
      I2C_PORT&=~(1<<SCL_LN);
    }
  __delay_cycles(F_CLK/I2C_T);
}

void sda_set(unsigned char set_c)
{
  if(set_c)
    {
      I2C_DDR&=~(1<<SDA_LN);
      I2C_PORT&=~(1<<SDA_LN);
    }
  else
    {
      I2C_DDR|=(1<<SDA_LN);
      I2C_PORT&=~(1<<SDA_LN);
    }
  __delay_cycles(F_CLK/I2C_T);
}

void i2c_init(void)
{
  I2C_DDR&=~(1<<SDA_LN);
  I2C_DDR&=~(1<<SCL_LN);
  I2C_PORT&=~(1<<SDA_LN);
  I2C_PORT&=~(1<<SCL_LN);
  i2c_error=0;
}

void i2c_start(void)
{
  if(i2c_error)
    return;
  scl_set(1);
  sda_set(0);
  scl_set(0);
}

void i2c_stop(void)
{
  sda_set(0);
  scl_set(1);
  sda_set(1);
  if(i2c_error)
  i2c_init();
}

void i2c_write(unsigned char byte)
{
  if(i2c_error)
    return;
  for(unsigned char i=0;i<8;i++)
    {
      if(byte&0x80)
        sda_set(1);
      else
        sda_set(0);
      scl_set(1);
      scl_set(0);
      byte<<=1;
    }
  sda_io(IN_LN);
  scl_set(1);
  i2c_error=in_sda();
  scl_set(0);
  sda_io(OUT_LN);
}

unsigned char i2c_read(unsigned char last_byte)
{
  unsigned char data=0;
  unsigned char mask=0x80;
  if(i2c_error)
    return 0;
  sda_io(IN_LN);
  for(unsigned char i=0;i<8;i++)
    {
      scl_set(1);
      if(in_sda())
        data=data+mask;
      mask>>=1;
      scl_set(0);
    }
  sda_io(OUT_LN);
  if(last_byte)
    sda_set(0);
  else
    sda_set(1);
  scl_set(1);
  scl_set(0);
  return data;
}


