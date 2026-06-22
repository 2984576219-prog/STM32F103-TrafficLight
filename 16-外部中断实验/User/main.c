#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "key.h"
#include "exti.h"

int main()
{
	u8 i=0;
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	LED_Init();
	KEY_Init();
	My_EXTI_Init();  
	
	while(1)
	{	
		i++;
		if(i%10==0)
			LED0=!LED0;

		delay_ms(10);
	}
}
