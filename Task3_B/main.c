#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"
#include "TM4C123.h"
#include "driverlib/timer.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"
#include "inc/hw_gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"

#include "SysTick.h"
#include "DIO.h"
#include "LCD.h"
#include "KEYPAD.h"
#include <stdio.h>
#include "string.h"
#include <stdlib.h>

unsigned char timersflag;
int secondsflag = 0;
int s;
int intToStr(int x, char str[], int d)
{
    bool negative = false;
    if(x < 0){
      x = x*-1;
      negative = true;
    }
    int n = 0;
    int value = x;
    while(value){
      value = value/10;
      n++;
    }
    if(negative){
      n = n+1;
      str[0] = '-';
    }
    for (int j = n-1; j >= (int)negative; j--) {
        str[j] = (x % 10) + '0';
        x = x / 10;
    }
    int i = n;
    int index = i;
    str[i++] = '\0';
    while (i < d)
        str[i++] = 0;
    return index;
}

void floatToStr(float x, char str[], int d){
  int value = (int)x;
  float decimal = x-value;
  int i = intToStr(value, str, d);
  float check = decimal;
  int checkInt = (int)check;
  while(check != checkInt){
    check = check*10;
    checkInt = (int)check;
  }
  if(checkInt != 0){
    str[i++] = '.';
    intToStr(checkInt, str+i, 20);
  }
  
}
 

char *sliceString(char *str, int start, int end)
{

    int i;
    int size = (end - start) + 2;
    char *output = (char *)malloc(size * sizeof(char));

    for (i = 0; start <= end; start++, i++)
    {
        output[i] = str[start];
    }

    output[size] = '\0';

    return output;
}
void calculator();
unsigned char internal_state = '*';
unsigned char GET_VALUE = '!';
unsigned char interrupt ;

void states_handler(){
  interrupt = '1';
  internal_state = '*';
  GPIO_PORTF_ICR_R = 0xff;
  if(timersflag == 't')
  {
  TimerDisable(TIMER0_BASE, TIMER_A);
  }
  else if (timersflag == 's')
  {
    secondsflag = 1;
  TimerDisable(TIMER1_BASE, TIMER_A);

  }
}

void buttoninit(){

    SYSCTL_RCGCGPIO_R |= 0x00000020;
    while((SYSCTL_PRGPIO_R&0x00000020) == 0){};
    GPIO_PORTF_LOCK_R = 0x4C4F434B;
    GPIO_PORTF_CR_R = 0x1F;
    GPIO_PORTF_DIR_R = 0x0E;
    GPIO_PORTF_PUR_R = 0x11;
    GPIO_PORTF_DEN_R = 0x11;
    GPIO_PORTF_DATA_R = 0x0U; 
    
    Clear_Bit(GPIO_PORTF_IS_R,PF4); 
    Clear_Bit(GPIO_PORTF_IBE_R,PF4); 
    Clear_Bit(GPIO_PORTF_IEV_R,PF4); 
    Set_Bit(GPIO_PORTF_ICR_R,PF4);
    Set_Bit(GPIO_PORTF_IM_R,PF4); 
    Set_Bit(NVIC_EN0_R,30); 
    NVIC_PRI7_R = NVIC_PRI7_R | 0x80000;  
    
}
void PortaHandler()
{
  //interrupt caused by PA2
  //PA2 pause
  if((GPIO_PORTA_MIS_R  & 0x04) == 0x04)
  {
      TimerDisable(TIMER1_BASE, TIMER_A);
      Set_Bit(GPIO_PORTA_ICR_R,PF2); 
      
  }
  //interrupt caused by PA3
  //PA3 reset
  else if((GPIO_PORTA_MIS_R & 0x08) == 0x08)
  {
        secondsflag = 1;
        TimerDisable(TIMER1_BASE, TIMER_A);
       interrupt = '1';
       internal_state = '/';
       Set_Bit(GPIO_PORTA_ICR_R,PF3);  
  }
}
void interrupt_porta(unsigned char pin)
{
    DIO_Init(PORTA,pin,IN,pin);
    Clear_Bit(GPIO_PORTA_IS_R,pin); 
    Clear_Bit(GPIO_PORTA_IBE_R,pin); 
    Clear_Bit(GPIO_PORTA_IEV_R,pin); 
    Set_Bit(GPIO_PORTA_ICR_R,pin); 
    Set_Bit(GPIO_PORTA_IM_R,pin); 
    Set_Bit(NVIC_EN0_R,PF0);  
    NVIC_PRI0_R = NVIC_PRI0_R | 0x60;
}

void welcome(){
    LCD_Command(clear_display);
    LCD_String("A-Calc B-Timer  C-StopWatch");
    LCD_Command(jump_second_line);
    LCD_String("C-StopWatch");
    GET_VALUE = '!';
    while(GET_VALUE == '!'){
      GET_VALUE = keypad_read();
      if(GET_VALUE != '+' && GET_VALUE != '-' && GET_VALUE != '/'){
        GET_VALUE = '!';
      } 
   }
   internal_state = GET_VALUE;
}

void ftoa(float n, char* res, int afterpoint)
{
    int ipart = (int)n;
    float fpart = n - (float)ipart;
 
    int i = intToStr(ipart, res, 0);
 
    if (afterpoint != 0) {
        res[i] = '.';
        fpart = fpart * pow(10, afterpoint);
 
        intToStr((int)fpart, res + i + 1, afterpoint);
    }
}
void seconds_display(int rest_seconds){

  char num [5] = {'0','0',':','0','0'};
  
  if(rest_seconds <60){
    if (rest_seconds < 10){
      char num_temp[2];
      ftoa(rest_seconds,num_temp,0);
      num[4] = num_temp[0];
    }else {
      char num_temp[2];
      ftoa(rest_seconds,num_temp,0);
      num[3] = num_temp[0];
      num[4] = num_temp[1];
    }
  
  LCD_Command(clear_display);
  LCD_String(num);
  }else{
  
  // check if we have minutes or just seconds
  int minutes = 0 ;
  if(rest_seconds>59){
    while(rest_seconds > 59){
      rest_seconds = rest_seconds - 60 ;
      minutes ++ ;
    }  
  }
  
  // print minutes
  if(minutes > 9){
    char num_temp[2];
    ftoa(minutes,num_temp,0);
    num[0] = num_temp[0];
    num[1] = num_temp[1];
  }else {
    char num_temp[2];
    ftoa(minutes,num_temp,0);
    num[1] = num_temp[0];

  }
  
  // print seconds
  if(rest_seconds>9){
    char num_temp[2];
    ftoa(rest_seconds,num_temp,0);
    num[3] = num_temp[0];
    num[4] = num_temp[1];
  }else {
    char num_temp[2];
    ftoa(rest_seconds,num_temp,0);
    num[4] = num_temp[0];
  }
  LCD_Command(clear_display);
  LCD_String(num);
  }
  
}

void handler()
{
  static int count =0;
  count ++;
  if(count == 3){
  s--;
  seconds_display(s);
  count = 0;
  }
  TIMER0_ICR_R = 0x1 ;
  if(s == 0){
    //SysTickDisable();
    TimerDisable(TIMER0_BASE, TIMER_A);
    DIO_WritePin(PORTD,PF3,1);
    delay(3000);
    DIO_WritePin(PORTD,PF3,0);
    LCD_Command(clear_display);
  }

}

void tim()
{
  timersflag = 't';
   SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0)){}
  TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
  IntEnable(INT_TIMER0A);
  TimerDisable(TIMER0_BASE,TIMER_A);
  
  //initiating portD pin 3 for the timer led or buzzer
  DIO_Init(PORTD,PF3,OUT,0X08);
  DIO_WritePin(PORTD,PF3,0);
  
  //TimerIntRegister(TIMER0_BASE,TIMER_A,ISR_Timer0Handler);
  TimerConfigure(TIMER0_BASE,TIMER_CFG_PERIODIC);
  TimerControlStall(TIMER0_BASE,TIMER_A,true);
  TimerLoadSet(TIMER0_BASE,TIMER_A,16000000);
  
  char data[4];
    LCD_Command(clear_display);
    LCD_String("  :");
    LCD_Command(move_cursor_left);
    LCD_Command(move_cursor_left);
    LCD_Command(move_cursor_left);
    int count = 1;
    while(count < 5)
    {
      char input;
      delay(300);
      input  = keypad_read();
      
      if(input != '!')
      {
     
        LCD_Char(input);
        data[count-1]=input;
        count++;
           if(count==3)
        {
          LCD_Command(move_cursor_right);
        }
      }
    }
   int m1 = (int) data[0] -48;
   int m2 = (int) data[1] -48;
   int s1 = (int) data[2] -48 ;
   int s2 = (int) data[3] -48;
   int minutes = m1 *10 + m2;
   int seconds = s1 *10 + s2;
   
   s = minutes * 60 +seconds;
   TimerEnable(TIMER0_BASE,TIMER_A);
  while(interrupt == '0')
  {

  }
}

void stopwatch_handler()
{
  
  LCD_Command(move_cursor_left);
  LCD_Command(move_cursor_left);
  LCD_Command(move_cursor_left);
  LCD_Command(move_cursor_left);
  LCD_Command(move_cursor_left);

  static int seconds = 0 ;
  static int count = 0;
    if(secondsflag ==1)
  {
    seconds = 0;
    secondsflag = 0;
  }
  count ++ ;
  if(count == 3){
  seconds++;
  seconds_display(seconds);
  count =0;
  }
  TIMER1_ICR_R = 0x1 ;
  if(read_pin(PORTF,PF0) == 0 ){
    delay(50);
    if(read_pin(PORTF,PF0) == 0){
    TimerDisable(TIMER1_BASE, TIMER_A);
    //seconds_display(0);
    LCD_Command(clear_display);
    //secondsflag =1;
    
    interrupt = '1';
    internal_state ='/';
    
    }
  }
  
  if(read_pin(PORTF,PF4) == 0 ){
    delay(50);
    if(read_pin(PORTF,PF4) == 0){
    TimerDisable(TIMER1_BASE, TIMER_A);
    }
  }
  
  
}
void sw()
{
  interrupt_porta(PF2);
  interrupt_porta(PF3);
  
   timersflag = 's';
  LCD_Command(clear_display);
  LCD_Command(cursor_off);
     DIO_Init(PORTF,PF0,IN,0x1F);
   DIO_Init(PORTF,PF4,IN,0x1F);
   //IntRegister(,reset_handler());
    
   SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
   while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)){}
   
   GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_2);
   SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
   while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER1)){}
   TimerConfigure(TIMER1_BASE,TIMER_CFG_PERIODIC);
   TimerLoadSet(TIMER1_BASE, TIMER_A, 16000000);
   TimerIntEnable(TIMER1_BASE,TIMER_TIMA_TIMEOUT);
   TimerIntRegister(TIMER1_BASE,TIMER_A, stopwatch_handler);
   IntMasterEnable();
   
   LCD_String("00:00");
  LCD_Command(cursor_first);
  
  char in;
  static int counter = 0;
  
   char nums [4][4] = {
      {'1','2','3','+'},
      {'4','5','6','-'},
      {'7','8','9','/'},
      {'*','0','#','='},
    }; 
   while(in !='+'){
   for(int i =4; i<8;i++)
   {
     // reset pins
     DIO_WritePin(PORTC,4,0);
     DIO_WritePin(PORTC,6,0);
     DIO_WritePin(PORTC,5,0);
     DIO_WritePin(PORTC,7,0);
     
     // write desired pin
     DIO_WritePin(PORTC,i,1);
     
     for(int j = 0; j<4; j++)
     {

       if(read_pin(PORTE,j) == 1)
       {
          char letter = nums[j][i-4];
          if(letter == '+')
          {
               in = letter;
               TimerEnable(TIMER1_BASE, TIMER_A);
          }
          
       }
     }
   }
   delay(200);
   }
  while(interrupt=='0')
  {
    
  }
  LCD_Command(lcd_cursor_on);
}

int main()
{
  __asm("CPSIE I");   
   SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
   LCD_Init();
   LCD_Command(clear_display); //clear display
   LCD_Command(lcd_cursor_on); // lcd & cursor on 
   keypad_init();
   buttoninit();   
   
   while(1){
     interrupt = '0';
      switch (internal_state){
        case '*':
          interrupt = '0';
          welcome();
          delay(200);
          break;
          
        case '+':
          GET_VALUE = '!';
          calculator();
          delay(200);
          break;
        case '-':
          tim();
          delay(200);
          break;
          
        case '/':
          sw();
          break;
          
         default:
            break;
       }
   }
   
   return 0;
}


void calculator()
{
  static unsigned char getback='1';
  LCD_Command(clear_display);
  char data[32] ;
  int currentIndex = 0;
  float x =0;
  float y = 0;
  int state_var = 0;
  unsigned char sign;
   while(interrupt == '0')
   {
     char input = keypad_read();
     delay(250);
    if(input != '!'){
      LCD_Char(input);
    }else{
      continue;
    }
     switch(state_var)
      {
        case 0:
          if(input == '+' || input == '-' || input =='/'||input == '*')
            {
                sign = input;
                state_var = 1;
            }
            else{ 
                x = x*10 + ((int)input - 48);
             }
          break;
           case 1:
              if(input == '=')
              { 
                char string_ans[20];
                switch(sign)
                {
                  case '+':
                      x = x + y;
                      floatToStr(x, string_ans, 20);
                      LCD_String(string_ans);
                      break;
                  case '-':
                      x = x-y;
                      floatToStr(x, string_ans, 20);
                      LCD_String(string_ans);
                      break;
                  case '*':
                      x = x * y;
                      floatToStr(x, string_ans, 20);
                      LCD_String(string_ans);
                    break;
                  case '/':
                    x = x / y;
                    floatToStr(x, string_ans, 20);
                    LCD_String(string_ans);
                   break;
                }
                delay(1000);
                LCD_Command(clear_display);
                LCD_String(string_ans);
                state_var = 0;
                y=0;
              }else if(input == '-'){
                sign = '+';
              }else{
                y = y*10 + ((int)input - 48);
              }
             break;
                
       }
    }
}