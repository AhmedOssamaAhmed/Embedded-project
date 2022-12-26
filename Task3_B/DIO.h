
#ifndef __DIO_H_
#define __DIO_H_

#include "bitwise_operation.h"
#include "tm4c123gh6pm.h"


#define PORTA 0
#define PORTB 1
#define PORTC 2
#define PORTD 3
#define PORTE 4
#define PORTF 5

#define PF0 0
#define PF1 1
#define PF2 2
#define PF3 3 
#define PF4 4
#define PF5 5
#define PF6 6
#define PF7 7

#define  IN 0
#define  OUT 1

void DIO_Init(int port, int bit, int dir,int value_commit);
void DIO_WritePin(int port,int bit, int value);
void DIO_WritePort(int port,int value);
int read_pin(int port,int pin);
int read_port(int port);
void delay(int N);
int toggle_pin(int port,int bit);


#endif