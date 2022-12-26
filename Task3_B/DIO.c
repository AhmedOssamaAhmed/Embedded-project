#include "DIO.h"

void DIO_Init(int port, int bit, int dir,int value_commit){
  
    Set_Bit(SYSCTL_RCGCGPIO_R,port);
    while((SYSCTL_PRGPIO_R & (1<<port)) == 0){}; 
   switch(port){
   //****************** PORT A ************************   
    case PORTA:
      Set_Bit(GPIO_PORTA_DEN_R,bit);
      if(dir ==1)
      {
        Set_Bit(GPIO_PORTA_DIR_R,bit);
      }
      else if(dir ==0)
      {
        Clear_Bit(GPIO_PORTA_DIR_R,bit);
        Set_Bit(GPIO_PORTA_PDR_R,bit);
      }
      break;
   //****************** PORT B ************************
    case PORTB:
      GPIO_PORTB_LOCK_R = 0X4C4F434B;
      // GPIO_PORTB_CR_R = value_commit;
      
      if(dir ==1)
      {
        Set_Bit(GPIO_PORTB_DIR_R,bit);
      }
      else if(dir ==0)
      {
        Clear_Bit(GPIO_PORTB_DIR_R,bit);
        //Set_Bit(GPIO_PORTB_PDR_R,bit);
      }
      Set_Bit(GPIO_PORTB_DEN_R,bit);
      break;
   //****************** PORT C ************************
    case PORTC:
      GPIO_PORTC_LOCK_R = 0X4C4F434B;
  //    GPIO_PORTC_CR_R = value_commit;
      if(dir ==1)
      {
        Set_Bit(GPIO_PORTC_DIR_R,bit);
      }
      else if(dir ==0)
      {
        Clear_Bit(GPIO_PORTC_DIR_R,bit);
        Set_Bit(GPIO_PORTC_PDR_R,bit);
      }   
      Set_Bit(GPIO_PORTC_DEN_R,bit);
      break;
   //****************** PORT D ************************
    case PORTD:
      GPIO_PORTD_LOCK_R = 0X4C4F434B;
      GPIO_PORTD_CR_R = value_commit;
      Set_Bit(GPIO_PORTD_DEN_R,bit);
      if(dir ==1)
      {
        Set_Bit(GPIO_PORTD_DIR_R,bit);
      }
      else if(dir ==0)
      {
        Clear_Bit(GPIO_PORTD_DIR_R,bit);
        Set_Bit(GPIO_PORTD_PDR_R,bit);
      }  
      break;
   //****************** PORT E ************************
    case PORTE:
      GPIO_PORTE_LOCK_R = 0X4C4F434B;
      GPIO_PORTE_CR_R = value_commit;
      Set_Bit(GPIO_PORTE_DEN_R,bit);
      if(dir ==1)
      {
        Set_Bit(GPIO_PORTE_DIR_R,bit);
      }
      else if(dir ==0)
      {
        Clear_Bit(GPIO_PORTE_DIR_R,bit);
        Set_Bit(GPIO_PORTE_PDR_R,bit);
      }
      break;
   //****************** PORT F ************************     
    case PORTF:
      GPIO_PORTF_LOCK_R = 0X4C4F434B;
      GPIO_PORTF_CR_R = value_commit;
      Set_Bit(GPIO_PORTF_DEN_R,bit);
      if(dir ==1)
      {
        Set_Bit(GPIO_PORTF_DIR_R,bit);
      }
      else if(dir ==0)
      {
        Clear_Bit(GPIO_PORTF_DIR_R,bit);
        Set_Bit(GPIO_PORTF_PUR_R,bit);
      }
      
    default:
      break;
    }
}


void DIO_WritePin(int port,int bit, int value){
    switch(port)
    {
    case PORTA:
      if(value == 1)
      {
        Set_Bit(GPIO_PORTA_DATA_R,bit);
      }
      else if (value ==0)
      {
        Clear_Bit(GPIO_PORTA_DATA_R,bit);
      }
      break;
    case PORTB:
      if(value == 1)
      {
        Set_Bit(GPIO_PORTB_DATA_R,bit);
      }
      else if (value ==0)
      {
        Clear_Bit(GPIO_PORTB_DATA_R,bit);
      }
      break;
    case PORTC :
      
      if(value == 1)
      {
        Set_Bit(GPIO_PORTC_DATA_R,bit);
      }
      else if (value ==0)
      {
        Clear_Bit(GPIO_PORTC_DATA_R,bit);
      }
      
    break;
        
    case PORTD:
      if(value == 1)
      {
        Set_Bit(GPIO_PORTD_DATA_R,bit);
      }
      else if (value ==0)
      {
        Clear_Bit(GPIO_PORTD_DATA_R,bit);
      }
      break;
    case PORTE:
      if(value == 1)
      {
        Set_Bit(GPIO_PORTE_DATA_R,bit);
      }
      else if (value ==0)
      {
        Clear_Bit(GPIO_PORTE_DATA_R,bit);
      }
      break;
    case PORTF:
      if(value == 1)
      {
        Set_Bit(GPIO_PORTF_DATA_R,bit);
      }
      else if (value ==0)
      {
        Clear_Bit(GPIO_PORTF_DATA_R,bit);
      }
      break;
    default:
      break; 
    }
}
void DIO_WritePort(int port,int value){
  switch(port){
      case PORTA:
        GPIO_PORTA_DATA_R = value;
        break;
      case PORTB:
        GPIO_PORTB_DATA_R = value;
        break;
      case PORTC:
        GPIO_PORTC_DATA_R = value;
        break;
      case PORTD:
        GPIO_PORTD_DATA_R = value;
        break;
      case PORTE:
        GPIO_PORTE_DATA_R = value;
        break;
      case PORTF:
        GPIO_PORTF_DATA_R = value;
      default:
        break;
  }
}
int read_pin(int port,int bit){
  switch(port)
  {
  case PORTA:
    return Get_Bit(GPIO_PORTA_DATA_R,bit);
  case PORTB:
    return Get_Bit(GPIO_PORTB_DATA_R,bit);
  case PORTC:
    return Get_Bit(GPIO_PORTC_DATA_R,bit);
  case PORTD:
    return Get_Bit(GPIO_PORTD_DATA_R,bit);
  case PORTE:
    return Get_Bit(GPIO_PORTE_DATA_R,bit);
  case PORTF:
    return Get_Bit(GPIO_PORTF_DATA_R,bit);
  default:
    break;
  }
  
}

int read_port(int port)
{
  switch(port){
  case PORTA:
    return GPIO_PORTA_DATA_R;
  case PORTB:
    return GPIO_PORTB_DATA_R;
  case PORTC:
    return GPIO_PORTC_DATA_R;
  case PORTD:
    return GPIO_PORTD_DATA_R;
  case PORTE:
    return GPIO_PORTE_DATA_R;
  case PORTF:
    return GPIO_PORTF_DATA_R;
  default:
    break;
  }
 
}
int toggle_pin(int port,int bit){
  switch(port)
  {
  case PORTF:
    return Toggle_Bit(GPIO_PORTF_DATA_R,bit);
  }
}

void delay(int N){
  for(int i=0; i<N ; i++){
    for(int k=0; k<3160; k++){}
  }
}