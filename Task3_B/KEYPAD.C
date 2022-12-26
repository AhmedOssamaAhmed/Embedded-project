#include "KEYPAD.h"
#include "DIO.h"
#include "LCD.h"
unsigned char keypad_arr[4][4] = {
  {'1','2','3','+'},
  {'4','5','6','-'},
  {'7','8','9','/'},
  {'*','0','#','='}
};
void keypad_init(void){
  //rows
  DIO_Init(PORTE,PF0,IN,0x0f);
    DIO_Init(PORTE,PF1,IN,0x0f);
      DIO_Init(PORTE,PF2,IN,0x0f);
        DIO_Init(PORTE,PF3,IN,0x0f);
  //columns init
  DIO_Init(PORTC,PF4,OUT,0XF0);
  DIO_Init(PORTC,PF5,OUT,0XF0);
  DIO_Init(PORTC,PF6,OUT,0XF0);
  DIO_Init(PORTC,PF7,OUT,0XF0);
}

unsigned char keypad_read(void){

  delay(15);
  unsigned long portd_value = 0;
  while(1)
  {
      DIO_WritePort(PORTC,0x0D);
      for (int j = 4 ; j<8 ; j++)
      {
          DIO_WritePin(PORTC,j,1);

          portd_value = read_port(PORTE);
          
          if(portd_value != 0x00)
          {
          switch(portd_value)
            {
                case 0x01:
                  printf("%d",portd_value);
                  return keypad_arr[0][j-4];
                  break;
                case 0x02:
                  return keypad_arr[1][j-4];
                  break;
                case 0x04:
                  return keypad_arr[2][j-4];
                  break;
                case 0x08:
                  return keypad_arr[3][j-4];
                  break;
                default:
                  break;
            }
          }
        DIO_WritePort(PORTC,0x0d);
      }
      return '!';
      break;
  }
}

uint8 PrintNow(short input)
{
  switch(input)
  {
  case 1:
    LCD_Char('1');
    return '1';
    break;
  case 2:
    LCD_Char('2');
    return '2';
    break;
  case 3:
    LCD_Char('3');
    return '3';
    break;
  case 4:
    LCD_Char('4');
    break;
  case 5:
    LCD_Char('5');
    break;
  case 6:
    LCD_Char('6');
    break;
  case 7:
    LCD_Char('7');
    break;
  case 8:
    LCD_Char('8');
    break;
  case 9:
    LCD_Char('9');
    break;
  case 0:
    LCD_Char('0');
    break;
  default:
    return '!';
    break;
      
  }
}