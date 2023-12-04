/**
  ******************************************************************************
  * @file    Project/STM32F0xx_StdPeriph_Templates/main.c 
  * @author  Windy Albert
  * @version V1.0.0
  * @date    03-December-2023
  * @brief   Main program body
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include "minos.h"                                   /* Header file for MinOS */

/** @addtogroup STM32F0xx_StdPeriph_Templates
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
extern uint32_t Image$$INIT$$Base;
extern uint32_t Image$$INIT$$Limit;
extern uint32_t Image$$TASK$$Base;
extern uint32_t Image$$TASK$$Limit;

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
    RCC_ClocksTypeDef RCC_Clocks;
    init_fnc_t *init_fnc ;
    Task_TableTypeDef *task_tbl;
    
	__disable_irq();
    
    /* SysTick end of count event each 1ms */
    RCC_GetClocksFreq(&RCC_Clocks);
    SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
    
    for( init_fnc = (init_fnc_t*)&Image$$INIT$$Base;        \
         init_fnc < (init_fnc_t*)&Image$$INIT$$Limit;       \
         init_fnc ++ )
    {
        if( (*init_fnc)() )
        {
            while(1);
        }
    }

    /**************************************************************************/
    /* Add your application code here *****************************************/
    /**************************************************************************/
    for( task_tbl = (Task_TableTypeDef*)&Image$$TASK$$Base;     \
         task_tbl < (Task_TableTypeDef*)&Image$$TASK$$Limit;    \
         task_tbl ++ )
    {
        TaskCreate( task_tbl->p_task, task_tbl->p_stack );
    }
    
    OSStart();
    return 0;
}

/**
  * @}
  */


/************************ (C) COPYRIGHT Windy Albert ***********END OF FILE****/
