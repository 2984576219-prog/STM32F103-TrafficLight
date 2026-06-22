#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "key.h"
#include "exti.h"
#include "timer.h"

int main()
{
	u8 i=0;
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  /* 中断优先级分组：2组 */
	LED_Init();
	KEY_Init();
	My_EXTI_Init();  
	
	/* 初始化TIM2定时器 */
	/* ARR=71, PSC=999时，计数器周期 = (71+1) * (999+1) / 72MHz = 1ms */
	Timer2_Init(71, 999);
	Timer2_Start();
	
	while(1)
	{
		/* 所有处理都在中断中进行 */
		i++;
		if(i%10==0)
			LED0=!LED0;

		delay_ms(10);
	}
}
