/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STEPPER_H
#define __STEPPER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "main.h"
/* Define   ------------------------------------------------------------------*/


void STEPPER_Init();
void STEPPER_SetStepToDo(int32_t nstep);
int32_t STEPPER_GetStepToDo();
void STEPPER_DoStep();
//void STEPPER_SetIdle();

#ifdef __cplusplus
}
#endif


#endif
