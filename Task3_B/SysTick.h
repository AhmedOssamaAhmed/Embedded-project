#ifndef __SysTick_H__
#define __SysTick_H__
#include "stdint.h"
#include "stdbool.h"

void SysTickDisable();
void SysTickEnable();
uint32_t SysTickPeriodGet();
void SysTickPeriodSet(uint32_t Period);
uint32_t SysTickValueGet();
bool SysTick_Is_Time_Out();


#endif