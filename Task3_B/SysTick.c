#include "DIO.h"
#include "SysTick.h"
#include "stdint.h"
#include "stdbool.h"
void SysTickDisable(){
  NVIC_ST_CTRL_R = 0x0;
}

void SysTickEnable(){
  NVIC_ST_CTRL_R = 0x7;
}

uint32_t SysTickPeriodGet(){
    return NVIC_ST_RELOAD_R;
}
void SysTickPeriodSet(uint32_t Period){
    NVIC_ST_RELOAD_R = ((Period)*(16*1000))-1;
}
uint32_t SysTickValueGet(){
  return   NVIC_ST_CTRL_R;
}

bool SysTick_Is_Time_Out(){
  
  return (SysTickValueGet() & 0x10000);
}