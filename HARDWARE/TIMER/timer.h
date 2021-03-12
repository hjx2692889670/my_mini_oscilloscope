#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
#include "GPIO_STM32F10x.h"

void TIM6_Int_Init(u16 arr,u16 psc);
void TIM3_Int_Init(u16 arr,u16 psc);

#endif
