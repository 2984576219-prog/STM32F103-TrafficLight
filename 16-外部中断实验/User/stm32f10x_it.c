/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "led.h"
#include "timer.h"
#include "exti.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* 交通灯状态定义 */
#define STATE_GREEN     0   /* 绿灯亮 */
#define STATE_YELLOW    1   /* 黄灯闪烁 */
#define STATE_RED       2   /* 红灯亮 */
#define STATE_EMERGENCY 3   /* 紧急模式 */

/* 黄灯快速闪烁周期 */
#define YELLOW_FLASH_TIME  500  /* 500ms */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile u8 traffic_state = STATE_GREEN;      /* 当前交通灯状态 */
volatile u16 state_counter = 0;               /* 状态计数器(毫秒) */
volatile u8 emergency_mode = 0;               /* 紧急模式标志 */
volatile u8 exti_debounce = 0;                /* 外部中断消抖计数 */
volatile u8 key_press_flag = 0;               /* KEY0按键按下标志 */

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  TIM2中断处理函数 - 交通灯时序控制
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        
        state_counter++;
        
        if (emergency_mode == 0)
        {
            /* 正常模式 */
            switch(traffic_state)
            {
                case STATE_GREEN:
                    LED0 = 0;  /* 绿灯常亮 */
                    LED1 = 1;  /* 黄灯熄灭 */
                    LED2 = 1;  /* 红灯熄灭 */
                    
                    if (state_counter >= 15000)  /* 15秒 */
                    {
                        traffic_state = STATE_YELLOW;
                        state_counter = 0;
                    }
                    break;
                    
                case STATE_YELLOW:
                    /* 黄灯闪烁 500ms亮、500ms灭 */
                    if ((state_counter % 1000) < 500)
                    {
                        LED1 = 0;  /* 黄灯亮 */
                    }
                    else
                    {
                        LED1 = 1;  /* 黄灯灭 */
                    }
                    LED0 = 1;  /* 绿灯熄灭 */
                    LED2 = 1;  /* 红灯熄灭 */
                    
                    if (state_counter >= 3000)  /* 3秒 */
                    {
                        traffic_state = STATE_RED;
                        state_counter = 0;
                    }
                    break;
                    
                case STATE_RED:
                    LED0 = 1;  /* 绿灯熄灭 */
                    LED1 = 1;  /* 黄灯熄灭 */
                    LED2 = 0;  /* 红灯常亮 */
                    
                    if (state_counter >= 10000)  /* 10秒 */
                    {
                        traffic_state = STATE_GREEN;
                        state_counter = 0;
                    }
                    break;
                    
                default:
                    break;
            }
        }
        else
        {
            /* 紧急模式 */
            LED2 = 0;  /* 红灯常亮 */
            LED0 = 1;  /* 绿灯熄灭 */
            
            /* 黄灯快速闪烁 250ms亮、250ms灭 */
            if ((state_counter % 500) < 250)
            {
                LED1 = 0;  /* 黄灯亮 */
            }
            else
            {
                LED1 = 1;  /* 黄灯灭 */
            }
        }
    }
}

/**
  * @brief  EXTI15_10中断处理函数 - 按键中断
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line15) != RESET)  /* KEY0按键对应PA15 */
    {
        exti_debounce++;  /* 消抖计数 */
        
        if (exti_debounce >= 2)  /* 消抖延迟2ms */
        {
            exti_debounce = 0;
            
            if (KEY1 == 0)  /* 确认按键按下 */
            {
                /* 切换紧急模式 */
                emergency_mode = ~emergency_mode;
                
                if (emergency_mode == 0)
                {
                    /* 退出紧急模式，恢复正常时序 */
                    traffic_state = STATE_GREEN;
                    state_counter = 0;
                }
                else
                {
                    /* 进入紧急模式 */
                    state_counter = 0;
                }
            }
        }
        
        EXTI_ClearITPendingBit(EXTI_Line15);
    }
}

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
