#ifndef _timer_H
#define _timer_H

#include "system.h"

/* 定时器配置 - TIM2用于交通灯时序 */
#define TRAFFIC_LIGHT_TIMER      TIM2
#define TRAFFIC_LIGHT_TIMER_IRQ  TIM2_IRQn
#define TRAFFIC_LIGHT_RCC        RCC_APB1Periph_TIM2

void Timer2_Init(u16 arr, u16 psc);
void Timer2_Start(void);
void Timer2_Stop(void);

#endif
