#include "timer.h"

/*******************************************************************************
* 函 数 名         : Timer2_Init
* 函数功能		   : TIM2定时器初始化
* 输    入         : arr自动重装值, psc预分频系数
* 输    出         : 无
* 说    明         : TIM2用于交通灯精准计时
*                   计数器周期 = (arr+1) * (psc+1) / 72MHz
*                   默认配置为1ms中断一次
*******************************************************************************/
void Timer2_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* 使能TIM2时钟 */
    RCC_APB1PeriphClockCmd(TRAFFIC_LIGHT_RCC, ENABLE);

    /* TIM2时基配置 */
    TIM_TimeBaseStructure.TIM_Period = arr;                    /* 自动重装值 */
    TIM_TimeBaseStructure.TIM_Prescaler = psc;                 /* 预分频系数 */
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;    /* 分频因子 */
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;/* 向上计数模式 */
    TIM_TimeBaseInit(TRAFFIC_LIGHT_TIMER, &TIM_TimeBaseStructure);

    /* 使能更新中断 */
    TIM_ITConfig(TRAFFIC_LIGHT_TIMER, TIM_IT_Update, ENABLE);

    /* NVIC配置 */
    NVIC_InitStructure.NVIC_IRQChannel = TRAFFIC_LIGHT_TIMER_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  /* 抢占优先级 */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;         /* 子优先级 */
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* 函 数 名         : Timer2_Start
* 函数功能		   : 启动TIM2
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void Timer2_Start(void)
{
    TIM_Cmd(TRAFFIC_LIGHT_TIMER, ENABLE);
}

/*******************************************************************************
* 函 数 名         : Timer2_Stop
* 函数功能		   : 停止TIM2
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void Timer2_Stop(void)
{
    TIM_Cmd(TRAFFIC_LIGHT_TIMER, DISABLE);
}
