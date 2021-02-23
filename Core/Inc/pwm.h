/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PWM_H
#define __PWM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "main.h"
/* Define   ------------------------------------------------------------------*/

enum PWM_ENABLED {pwm_disabled = 0,pwm_enabled};

/* Function prototypes -------------------------------------------------------*/
void PWM_Init(void);
int32_t PWM_Get_Max(void);
uint32_t PWM_Get_Freq(void);
void PWM_Set_Value(int32_t value_1, int32_t value_2);
void PWM_Enable();
void PWM_Disable();

#ifdef __cplusplus
}
#endif


#endif
