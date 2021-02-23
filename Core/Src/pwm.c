/* Includes ------------------------------------------------------------------*/
#include "pwm.h"
#include <stm32f4xx_hal_def.h>
#include <stm32f4xx_hal_rcc.h>
#include <stm32f4xx_hal_tim.h>

#define PWM_MAXDUTY 95 //Max dutycycle %

/* Private variables ---------------------------------------------------------*/
extern TIM_HandleTypeDef htim3;

int32_t PWM_maxValue;
int32_t PWM_minValue;
int32_t PWM_zeroValue;
uint32_t PWM_freq;
uint32_t PWM_enabled;


void PWM_Init(void)
{
	PWM_freq = (HAL_RCC_GetSysClockFreq()/htim3.Init.Period)/2;

	PWM_zeroValue = htim3.Init.Period/2;
	PWM_maxValue = PWM_zeroValue * PWM_MAXDUTY /100;
	PWM_minValue = -PWM_maxValue;
	PWM_enabled = pwm_disabled;

	PWM_Set_Value(0,0);
	PWM_Enable();

	if (HAL_TIM_Base_Start_IT(&htim3) != HAL_OK)
	{
		Error_Handler();
	}
}

int32_t PWM_Get_Max(void)
{
	return PWM_zeroValue;
}

uint32_t PWM_Get_Freq(void)
{
	return PWM_freq;
}

void PWM_Set_Value(int32_t value_1, int32_t value_2)
{
	uint32_t ccr1Value;
	uint32_t ccr2Value;
	uint32_t ccr3Value;
	uint32_t ccr4Value;

	if (value_1 > PWM_maxValue) value_1 = PWM_maxValue;
	if (value_1 < PWM_minValue) value_1 = PWM_minValue;

	ccr1Value = (uint32_t)(PWM_zeroValue + value_1);
	ccr2Value = (uint32_t)(PWM_zeroValue - value_1);

	if (value_2 > PWM_maxValue) value_2 = PWM_maxValue;
	if (value_2 < PWM_minValue) value_2 = PWM_minValue;

	ccr3Value = (uint32_t)(PWM_zeroValue + value_2);
	ccr4Value = (uint32_t)(PWM_zeroValue - value_2);

	(&htim3)->Instance->CCR1 = ccr1Value;
	(&htim3)->Instance->CCR2 = ccr2Value;
	(&htim3)->Instance->CCR3 = ccr3Value;
	(&htim3)->Instance->CCR4 = ccr4Value;

}

void PWM_Enable()
{
	if (PWM_enabled == pwm_disabled)
	{
		PWM_enabled = pwm_enabled;
		if (HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_1) != HAL_OK)
		{
			Error_Handler();
		}
		if (HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_2) != HAL_OK)
		{
			Error_Handler();
		}
		if (HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_3) != HAL_OK)
		{
			Error_Handler();
		}
		if (HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_4) != HAL_OK)
		{
			Error_Handler();
		}
	}
}

void PWM_Disable()
{
	if (HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_2) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_PWM_Stop_IT(&htim3,TIM_CHANNEL_3) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_4) != HAL_OK)
	{
		Error_Handler();
	}
	PWM_enabled = pwm_disabled;
}
