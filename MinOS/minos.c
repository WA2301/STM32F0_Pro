/**
  ******************************************************************************
  * @file    minos.c 
  * @author  Windy Albert
  * @version V2.0
  * @date    04-November-2019
  * @brief   This is the core file for MinOS, a NO GRAB OS for embedded system.
  *          The system CANNOT run over 49 days consecutively.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "minos.h"

OS_TCB *OSTCBCur;                    /* Pointer to currently running TCB      */
OS_TCB  OSTCBTbl[OS_MAX_TASKS];      /* Table of TCBs                         */

uint32_t SysTime = 0;

/**
  * @brief  All tasks MUST be created completed before OSStart().
  * @param  task     is a pointer to the task's code
  *         ptos     is a pointer to the task's top of stack.
  * @retval None
  */
void  TaskCreate(void (*task)(void), uint32_t *stk)
{ 
    static uint8_t task_num = 0;
    
    if( task_num >= OS_MAX_TASKS )
    {
        while(1);
    }
                                        /* Registers auto-saved on exception  */
    *(stk)    = (uint32_t)0x01000000L;  /* xPSR                               */
    *(--stk)  = (uint32_t)task;         /* Entry Point                        */
    *(--stk)  = (uint32_t)0xFFFFFFFEL;  /* R14 (LR) (So task CANNOT return)   */
    *(--stk)  = (uint32_t)0x12121212L;  /* R12                                */
    *(--stk)  = (uint32_t)0x03030303L;  /* R3                                 */
    *(--stk)  = (uint32_t)0x02020202L;  /* R2                                 */
    *(--stk)  = (uint32_t)0x01010101L;  /* R1                                 */
    *(--stk)  = (uint32_t)0x00000000L;  /* R0 : argument                      */
                                        /* Registers saved on PSP             */
    *(--stk)  = (uint32_t)0x07070707L;  /* R7   R8-R11 ??                     */
    *(--stk)  = (uint32_t)0x06060606L;  /* R6                                 */
    *(--stk)  = (uint32_t)0x05050505L;  /* R5                                 */
    *(--stk)  = (uint32_t)0x04040404L;  /* R4                                 */
    
    OSTCBCur                = &OSTCBTbl[task_num];
    OSTCBCur->OSTCBStkPtr   = stk;        /* Initialize the task's stack      */
    OSTCBCur->OSTCBNext     = &OSTCBTbl[0];
    OSTCBCur->OSTCBWakeTime = 0;
    
    if( task_num > 0 )
    {
        OSTCBTbl[task_num-1].OSTCBNext = OSTCBCur;
    }
    
    task_num++;
}

/**
  * @brief  Trigger the PendSV.
  * @param  None
  * @retval None
  */
__inline void __Sched (void) 
{ 
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}

/**
  * @brief  Before you can call OSStart(), and you MUST have created at least one task.
  * @param  None
  * @retval None
  */
__inline void OSStart (void)
{
    __NVIC_SetPriority ( PendSV_IRQn, 0xFF );  /** SCB->SHP[10] = 0xFF;      **/
    __set_PSP(0);
    __Sched();    
    __enable_irq();
}

/**
  * @brief  This function is called to delay execution of the currently running 
  *         task until the specified number of system ticks expires.
  * @param  ticks     is the time delay that the task will be suspended.
  * @retval None
  */
__inline void OSTimeDly( uint16_t ticks )
{ 
    OSTCBCur->OSTCBWakeTime = OSTime_Now() + ticks;
    while( OSTime_Now() < OSTCBCur->OSTCBWakeTime ) { 
        __Sched(); 
    }
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
__ASM void PendSV_Handler (void)
{
    extern  OSTCBCur    
    PRESERVE8
    
    CPSID I               /* Prevent interruption during context switch       */
    MRS   R0, PSP         /* PSP is process stack pointer                     */
    CMP R0, #0	          /* Skip register save the first time                */
	BEQ _nosave
   
    SUBS    R0, R0, #0x10  /* PUSH r4-7  to current process stack              */
    STM     R0!, {R4-R7}   /* STM = STMIA */
    SUBS    R0, R0, #0x10





    
    LDR   R1, =OSTCBCur   /* OSTCBCur->OSTCBStkPtr = PSP;                     */
    LDR   R1, [R1]        /*                                                  */
    STR   R0, [R1]        /* R0 is SP of process being switched out           */
                          /* Now, entire context of process has been saved    */
_nosave                   /*                                                  */
	LDR   R0, =OSTCBCur   /* OSTCBCur  = OSTCBCur->OSTCBNext;                 */
    LDR   R2, [R0]        /*                                                  */
    ADDS  R2, R2, #4   /*                                                  */
	LDR   R2, [R2]        /*                                                  */
	STR   R2, [R0]        /*                                                  */
						  /*                                                  */
    LDR   R0, [R2]        /* PSP = OSTCBCur->OSTCBStkPtr                      */
    

    LDM   R0!, {R4-R7}   /* POP r4-7  from new process stack  LDM = LDMIA     */
	                      /*                                                  */
    MSR   PSP, R0         /* Load PSP with new process SP                     */
	
    MOV   R0,  R14
    MOVS  R1,  #0x04
    ORRS  R0,  R1         /* Ensure exception return uses process stack       */
    MOV   R14, R0                                                               
	                      /*                                                  */
    CPSIE I               /*                                                  */
    BX    LR              /* Exception return will restore remaining context  */
                                
	ALIGN
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
    SysTime++;
}
/**************** (C) COPYRIGHT 2019 Windy Albert ******** END OF FILE ********/
