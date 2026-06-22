#include "exti.h"
#include "led.h"
#include "SysTick.h"
#include "key.h"


/*******************************************************************************
* 函 数 名         : My_EXTI_Init
* 函数功能		   : 外部中断初始化
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void My_EXTI_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef  EXTI_InitStructure;
	
	/* 使能AFIO时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	/* 配置GPIO管脚为EXTI中断输入 */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource15);  /* KEY0-PA15 */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource14);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource13);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource12);
	
	/* EXTI12-15 NVIC配置 */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;  /* EXTI中断通道 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;  /* 抢占优先级 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;     /* 子优先级 */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;        /* IRQ通道使能 */
	NVIC_Init(&NVIC_InitStructure);
	
	/* EXTI配置 */
	EXTI_InitStructure.EXTI_Line=EXTI_Line12|EXTI_Line13|EXTI_Line14|EXTI_Line15;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;  /* 下降沿触发 */
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}
