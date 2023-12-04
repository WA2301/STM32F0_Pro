/**
  ******************************************************************************
  * @file    minos.h 
  * @author  Windy Albert
  * @version V2.0
  * @date    04-November-2019
  * @brief   This is the head file for MinOS, a NO GRAB OS for embedded system.
  *          OSTaskCreate( task, stk ) @USAGE OSTaskCreate( Task00, 0x80 );
  *          void OSStart( void );     @USAGE OSStart( );
  *          void OSTimeDly( ticks );  @USAGE OSTimeDly( 1000 );
  *          OSWait( con )             @USAGE OSWait( var >= 1 );
  *          OSWaitTime( con,time );   @USAGE OSWaitTime( var >= 1,1000 );
  *          OSisTimeOut()             @USAGE if( OSisTimeOut() ){ //do sth }  
  ******************************************************************************
  */

#ifndef   _MINOS_H
#define   _MINOS_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"

#define OS_MAX_TASKS    10      /* Max.num of tasks in your app, MUST be >= 1 */

typedef struct os_tcb {
    uint32_t *OSTCBStkPtr;           /* Pointer to current top of stack       */
    struct os_tcb *OSTCBNext;        /* Pointer to next TCB in the TCB list   */
    uint32_t OSTCBWakeTime;          /* Next time to run this task            */
} OS_TCB;

extern void TaskCreate (void (*task)(void), uint32_t *ptos);
extern void __Sched (void);

extern uint32_t         SysTime;
extern OS_TCB          *OSTCBCur;
#define OSTime_Now()    SysTime
    
/******************************************************************************/
/* Public functions ----------------------------------------------------------*/
/******************************************************************************/        
extern void OSStart( void );
extern void OSTimeDly( uint16_t ticks );

#define OSWait( con )  while(!(con)){__Sched();}

#define OSWaitTime( con,time )                                      \
                { OSTCBCur->OSTCBWakeTime = OSTime_Now() + time;    \
                  while(!(con) ){                                   \
                    if( OSTime_Now()<OSTCBCur->OSTCBWakeTime ){     \
                        __Sched();                                  \
                    }else{                                          \
                        OSTCBCur->OSTCBWakeTime = 0;break;          \
                    }                                               \
                  }                                                 \
                }
#define OSisTimeOut() (OSTCBCur->OSTCBWakeTime==0)
                
#endif
                
/************* (C) COPYRIGHT 2019 Windy Albert *********** END OF FILE ********/
