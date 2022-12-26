
#ifndef __LCD_H__
#define __LCD_H__


#define LCD_Port GPIO_PORTB_DATA_R			/* Define LCD data port */
#define LCD_RS_PIN PF0				/* Define Register Select pin */
#define LCD_EN_PIN PF2 				/* Define Enable signal pin */
#define clear_display 0X01
#define jump_second_line 0XC0
#define lcd_cursor_on 0X0F
#define shift_right 0x1C
#define cursor_first 0x80
#define move_cursor_right 0x14
#define move_cursor_left 0x10
#define cursor_off 0x0c


#define uint8 unsigned char 

#define PORTB_Direct GPIO_PORTB_DATA_R


void LCD_Command( uint8 cmnd );


void LCD_Init (void);

void LCD_Char( uint8 data );

void LCD_String( uint8 str[] );

#endif