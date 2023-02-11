#include <iom2560.h>
#include <stdio.h>
#include <intrinsics.h>
#include <string.h>

#include "system.c"                                                             // System config
#include "led.c"                                                                // LED driver
#include "button.c"                                                             // Buttons driver
#include "usart.c"                                                              // USART driver
#include "timers.c"                                                             // Timers driver
#include "i2c_soft.c"                                                           // I2C Soft driver
#include "SSD1306.c"                                                            // Display driver

#include "k5l.c"                                                                // K5L2731CCA-D770 Flash chip driver 
#include "flash_operation.c"                                                    // Flash operation

#include "parsing.c"                                                            // USART parsing data from PC
#include "all_logic.c"                                                          // Main logic



void main( void )
{
  main_init();
  main_start();
  main_loop();
}
