#include "SSD1306.h"
#include "font.c"

void ssd1306_command(char command) 
{
    i2c_start();
    i2c_write((ssd1306_ADDRESS << 1) | WRITE); 
    i2c_write(CONTROL_BYTE_COMMAND);
    i2c_write(command);
    i2c_stop();  
}

void ssd1306_data(char data) 
{
    i2c_start();
    i2c_write((ssd1306_ADDRESS << 1) | WRITE); 
    i2c_write(CONTROL_BYTE_DATA);
    i2c_write(data);
    i2c_stop();  
}


void display_init()
{
    ssd1306_command(SSD1306_DISPLAYOFF);                    // 0xAE
    ssd1306_command(SSD1306_SETDISPLAYCLOCKDIV);            // 0xD5
    ssd1306_command(0x80);                                  // the suggested ratio 0x80
    ssd1306_command(SSD1306_SETMULTIPLEX);                  // 0xA8
    ssd1306_command(0x1F);
    ssd1306_command(SSD1306_SETDISPLAYOFFSET);              // 0xD3
    ssd1306_command(0x0);                                   // no offset
    ssd1306_command(SSD1306_SETSTARTLINE | 0x0);            // line #0
    ssd1306_command(SSD1306_CHARGEPUMP);                    // 0x8D
    ssd1306_command(0x14);
    ssd1306_command(SSD1306_MEMORYMODE);                    // 0x20
    ssd1306_command(0x00);                                  // 0x0 act like ks0108
    ssd1306_command(SSD1306_SEGREMAP | 0x1);
    ssd1306_command(SSD1306_COMSCANDEC);
    ssd1306_command(SSD1306_SETCOMPINS);                    // 0xDA
    ssd1306_command(0x02);
    ssd1306_command(SSD1306_SETCONTRAST);                   // 0x81
    ssd1306_command(0x8F);
    ssd1306_command(SSD1306_SETPRECHARGE);                  // 0xd9
    ssd1306_command(0xF1); 
    ssd1306_command(SSD1306_SETVCOMDETECT);                 // 0xDB
    ssd1306_command(0x40);
    ssd1306_command(SSD1306_DISPLAYALLON_RESUME);           // 0xA4
    ssd1306_command(SSD1306_NORMALDISPLAY);                 // 0xA6
    ssd1306_command(SSD1306_DISPLAYON);                     //--turn on oled panel
}



void display_clear()
{
  ssd1306_command(SSD1306_COLUMNADDR);
ssd1306_command(0);   // Column start address (0 = reset)
ssd1306_command(SSD1306_LCDWIDTH-1); // Column end address (127 = reset)
ssd1306_command(SSD1306_PAGEADDR);
ssd1306_command(0); // Page start address (0 = reset)
ssd1306_command(SSD1306_LCDHEIGHT/8); // Page end address
    for (int i=0; i<(SSD1306_LCDWIDTH*SSD1306_LCDHEIGHT/8); i++)
    {
     ssd1306_data(0);
    }
}


void display_invert(unsigned char i) {
  if (i) {
    ssd1306_command(SSD1306_INVERTDISPLAY);
  } else {
    ssd1306_command(SSD1306_NORMALDISPLAY);
  }
}

void goto_xy(unsigned char x, unsigned char y)
{
ssd1306_command(SSD1306_COLUMNADDR);
ssd1306_command(x);   // Column start address (0 = reset)
ssd1306_command(SSD1306_LCDWIDTH-1); // Column end address (127 = reset)
ssd1306_command(SSD1306_PAGEADDR);
ssd1306_command(y); // Page start address (0 = reset)
ssd1306_command(SSD1306_LCDHEIGHT/8); // Page end address
}

void printChar(unsigned char chr)
{
   // goto_xy(x,y); //установка координат
    
    for (int i=0; i<5; i++)
    {
      if(chr >191)
        ssd1306_data(font[chr - 96][i]);
      else
        ssd1306_data(font[chr - 32][i]);
    }
      ssd1306_data(0);
}

void printString(unsigned char x,unsigned char y,char *str)
{
  goto_xy(x,y); //установка координат
   for (int i=0; i < strlen(str); i++)
  {
   printChar(str[i]);
  }
}


void printStringF(unsigned char x,unsigned char y,char __flash *str)
{
  goto_xy(x,y); //установка координат
  char k;
while (k=*str++) 
 printChar(k);
}


void dig2 (unsigned char x, unsigned char y, unsigned char data)

{
    if (data > 99)
      data = 99;
  unsigned char Num1, Num2;
  Num1=0;
  while (data >= 10)  
  {
    data -= 10;  
    Num1++;  
  }
  Num2 = data;
  goto_xy(x,y);
  printChar(Num1+48);
  goto_xy(x+6,y);
  printChar(Num2+48);
}

void dig3 (unsigned char x, unsigned char y, unsigned int data)

{
    if (data > 999)
      data = 999;
  unsigned char Num1, Num2, Num3;
  Num1=Num2=0;
    while (data >= 100)  
  {
    data -= 100;  
    Num1++;  
  }
  while (data >= 10)  
  {
    data -= 10;  
    Num2++;  
  }
  Num3 = data;
  goto_xy(x,y);
  printChar(Num1+48);
  goto_xy(x+6,y);
  printChar(Num2+48);
  goto_xy(x+12,y);
  printChar(Num3+48);
}

void dig4 (unsigned char x, unsigned char y, unsigned int data)

{
    if (data > 9999)
      data = 9999;
  unsigned char Num1, Num2, Num3, Num4;
  Num1=Num2=Num3=0;
    while (data >= 1000)  
  {
    data -= 1000;  
    Num1++;  
  }
  while (data >= 100)  
  {
    data -= 100;  
    Num2++;  
  }
    while (data >= 10)  
  {
    data -= 10;  
    Num3++;  
  }
  Num4 = data;
  goto_xy(x,y);
  printChar(Num1+48);
  goto_xy(x+6,y);
  printChar(Num2+48);
  goto_xy(x+12,y);
  printChar(Num3+48);
  goto_xy(x+17,y);
  printChar(Num4+48);
}

void dig5 (unsigned char x, unsigned char y, unsigned long int data)

{
    if (data > 99999)
      data = 99999;
  unsigned char Num1, Num2, Num3, Num4,Num5;
  Num1=Num2=Num3=Num4=0;
    while (data >= 10000)  
  {
    data -= 10000;  
    Num1++;  
  }
  while (data >= 1000)  
  {
    data -= 1000;  
    Num2++;  
  }
    while (data >= 100)  
  {
    data -= 100;  
    Num3++;  
  }
      while (data >= 10)  
  {
    data -= 10;  
    Num4++;  
  }
  Num5 = data;
  goto_xy(x,y);
  printChar(Num1+48);
  goto_xy(x+6,y);
  printChar(Num2+48);
  goto_xy(x+12,y);
  printChar(Num3+48);
  goto_xy(x+18,y);
  printChar(Num4+48);
    goto_xy(x+24,y);
  printChar(Num5+48);
}


void dig10(unsigned char x, unsigned char y, unsigned long data)

{
    if (data > 4294967294)
      data = 4294967294;
  unsigned char Num1, Num2, Num3, Num4,Num5,Num6, Num7, Num8, Num9,Num10;
  Num1=Num2=Num3=Num4=Num5=Num6=Num7=Num8=Num9=0;
    
  while (data >= 1000000000)  
  {
    data -= 1000000000;  
    Num1++;  
  }
  while (data >= 100000000)  
  {
    data -= 100000000;  
    Num2++;  
  }
  while (data >= 10000000)  
  {
    data -= 10000000;  
    Num3++;  
  }
  while (data >= 1000000)  
  {
    data -= 1000000;  
    Num4++;  
  }
  while (data >= 100000)  
  {
    data -= 100000;  
    Num5++;  
  }
  while (data >= 10000)  
  {
    data -= 10000;  
    Num6++;  
  }
  while (data >= 1000)  
  {
    data -= 1000;  
    Num7++;  
  }
  while (data >= 100)  
  {
    data -= 100;  
    Num8++;  
  }
  while (data >= 10)  
  {
    data -= 10;  
    Num9++;  
  }
  Num10 = data;
    
  goto_xy(x,y);
  printChar(Num1+48);
  goto_xy(x+6,y);
  printChar(Num2+48);
  goto_xy(x+12,y);
  printChar(Num3+48);
  goto_xy(x+18,y);
  printChar(Num4+48);
    goto_xy(x+24,y);
  printChar(Num5+48);
  goto_xy(x+30,y);
  printChar(Num6+48);
  goto_xy(x+36,y);
  printChar(Num7+48);
  goto_xy(x+42,y);
  printChar(Num8+48);
  goto_xy(x+48,y);
  printChar(Num9+48);
    goto_xy(x+54,y);
  printChar(Num10+48);
}



