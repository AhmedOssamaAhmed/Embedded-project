#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "LCD.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "string.h"
#include"DIO.h"
#include"LCD.h"


void LCD_Command( uint8 cmnd )
{   
    
   // LCD_Port &= ~ (1<<LCD_EN_PIN);  //Disable pulse (BY me)
	LCD_Port = (LCD_Port & 0x0F) | (cmnd & 0xF0);/* Sending upper nibble */
	LCD_Port &= ~ (1<<LCD_RS_PIN);		/* RS=0, command reg. */
	LCD_Port |= (1<<LCD_EN_PIN);		/* Enable pulse */
        delay(2);                        //_delay_us(1);
	LCD_Port &= ~ (1<<LCD_EN_PIN);

	delay(2);                           //_delay_us(200);

	LCD_Port = (LCD_Port & 0x0F) | (cmnd << 4);/* Sending lower nibble */
	LCD_Port |= (1<<LCD_EN_PIN);
	delay(2);                        //_delay_us(1);
	LCD_Port &= ~ (1<<LCD_EN_PIN);
	delay(2);  //_delay_us(2);

}



void LCD_Init (void)  /* LCD Initialize function */
{
	for(uint8 i=PF0;i<=PF7;i++)
    {
        DIO_Init(PORTB,i,OUT,0xFF);
    }

	delay(50);    //              _delay_ms(20);		/* LCD Power ON delay always >15ms */
	
	LCD_Command(0x33);
	LCD_Command(0x32);	/* Send for 4 bit initialization of LCD  */
	LCD_Command(0x28);	/* 2 line, 5*7 matrix in 4-bit mode */
	LCD_Command(0x0c);	/* Display on cursor off */
	LCD_Command(0x06);	/* Increment cursor (shift cursor to right) */
	LCD_Command(clear_display);	/* Clear display screen */
}


void LCD_Char( uint8 data )
{
	LCD_Port = (LCD_Port & 0x0F) | (data & 0xF0);/* Sending upper nibble */
	LCD_Port |= (1<<LCD_RS_PIN);  /* RS=1, data reg. */
	LCD_Port|= (1<<LCD_EN_PIN);
	delay(2);                            //_delay_us(1);

	LCD_Port &= ~ (1<<LCD_EN_PIN);
	delay(2);                                      //_delay_us(200);
	LCD_Port = (LCD_Port & 0x0F) | (data << 4);  /* Sending lower nibble */
	LCD_Port |= (1<<LCD_EN_PIN);
	delay(2);                                                //_delay_us(1);
	LCD_Port &= ~ (1<<LCD_EN_PIN);
	delay(2);                                                           //_delay_ms(2);
}

void LCD_String( uint8 str[] )
{
  uint8 counter =0;
  
  while (str[counter]!='\0')
  {
    
    LCD_Char( str[counter] );
    counter++;
  }
    
}