/**
  ******************************************************************************
  * @author  Windy Albert
  * @date    08-April-2017
  * @brief   Task to ...
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include "minos.h"                                   /* Header file for MinOS */


/* Private functions ---------------------------------------------------------*/
//Cortex-M0 has NOT bit-banding

//#define GPIO_Reverse(GPIO, GPIO_Pin) \
//	if( GPIO->ODR & GPIO_Pin )   GPIO_ResetBits( GPIO, GPIO_Pin); \
//	else                         GPIO_SetBits( GPIO, GPIO_Pin);  
	
//		GPIO_SetBits( GPIOA, GPIO_Pin_5); 
//		GPIO_ResetBits( GPIOA, GPIO_Pin_5);

/**
  * @brief  This function config LED pin.
  * @param  None
  * @retval None
  */
INIT_REG(LED_Init)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable GPIO clocks */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

    /* LED GPIO Configuration ------------------------------------------------*/
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1 ;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;   /* Only in OUT/AF mode */
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;     /* Only in OUT/AF mode */
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;  /* Weak in OUT/AF mode */

    GPIO_Init(GPIOB , &GPIO_InitStructure);
	
	return 0;
}

/**
  * @brief  Task program, supposed to be loop forever.
  * @param  None
  * @retval None
  */
TASK_REG(LED_Task, 80)
{

	for(;;) {
		//TODO:
		
		GPIO_SetBits( GPIOB, GPIO_Pin_1);
		OSTimeDly(500); 
		GPIO_ResetBits( GPIOB, GPIO_Pin_1);
		
        
		
		OSTimeDly(500);
	}//for
}


/******************* (C) COPYRIGHT 2017 Windy Albert ***********END OF FILE****/
