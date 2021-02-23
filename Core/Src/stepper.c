/* Includes ------------------------------------------------------------------*/
#include "stepper.h"
#include "pwm.h"
#include <math.h>

#define STEPPER_VIN 24.0f   //[V] supply voltage
#define STEPPER_SPR 200.0f  //Step per round of the motor
						    //resolution is reduced by 4
							//so if SPR = 200 only 50 step to make one turn of the shaft
#define STEPPER_RPS 5.0f    //Round per second (Speed) 0.5<->5
#define STEPPER_VPEAK 16.0f //[V] Peak voltage in moving condition
							//it depends on speed, keep RMS current as low as possible (under 1.0A)
#define STEPPER_VOFF 4.0f   //[V] Max voltage in still condition
							//keep medium current as low as possible (under 1.0A)


#define STEPPER_BUFFERSIZE 1000
#define STEPPER_MINPOINTS 10

#ifndef PI_F
#define PI_F 3.14159265358979f
#endif

int32_t STEPPER_stepToDo = 0;
int32_t STEPPER_sinBuffer[STEPPER_BUFFERSIZE+1];
int32_t STEPPER_cosBuffer[STEPPER_BUFFERSIZE+1];
volatile uint32_t STEPPER_maxIndex = 0;
volatile uint32_t STEPPER_tick_count = 0;
volatile uint32_t STEPPER_sinIdle = 0;
volatile uint32_t STEPPER_cosIdle = 0;

void STEPPER_Init()
{
	float tempF;
	float maxValOn = 0;
	float maxValOff = 0;
	uint32_t i = 0;

	tempF = (float)PWM_Get_Max();
	tempF = tempF/STEPPER_VIN;
	maxValOn = STEPPER_VPEAK*tempF;
	maxValOff = STEPPER_VOFF*tempF;

	tempF = (float)PWM_Get_Freq();
	tempF = truncf(tempF*4.0/(STEPPER_SPR*STEPPER_RPS));

	if (tempF < (float)STEPPER_MINPOINTS) tempF = (float)STEPPER_MINPOINTS;
	if (tempF > (float)STEPPER_BUFFERSIZE) tempF = (float)STEPPER_BUFFERSIZE;

	STEPPER_maxIndex = (uint32_t)tempF;
	tempF = 2 * PI_F / tempF;
	for (i = 0; i < STEPPER_maxIndex; ++i)
	{
		STEPPER_sinBuffer[i] = (int32_t)roundf(maxValOn*sinf(tempF*(float)i));
		STEPPER_cosBuffer[i] = (int32_t)roundf(maxValOn*cosf(tempF*(float)i));
	}
	STEPPER_sinBuffer[STEPPER_maxIndex] = STEPPER_sinBuffer[0];
	STEPPER_cosBuffer[STEPPER_maxIndex]	= STEPPER_cosBuffer[0];

	STEPPER_sinIdle = 0;
	STEPPER_cosIdle = maxValOff;
}


void STEPPER_DoStep()
{
	volatile uint32_t sinValue;
	volatile uint32_t cosValue;
	volatile uint32_t bufferIndex;

	if  (STEPPER_stepToDo == 0)
	{
		sinValue = STEPPER_sinIdle;
		cosValue = STEPPER_cosIdle;
		STEPPER_tick_count = 0;
	}
	else
	{
		STEPPER_tick_count = STEPPER_tick_count + 1;
		if (STEPPER_stepToDo > 0)
		{
			bufferIndex = STEPPER_tick_count;
			sinValue = STEPPER_sinBuffer[bufferIndex];
			cosValue = STEPPER_cosBuffer[bufferIndex];

			if (bufferIndex == STEPPER_maxIndex)
			{
				STEPPER_tick_count = 0;
				STEPPER_stepToDo = STEPPER_stepToDo - 1;
			}
		}
		else if (STEPPER_stepToDo < 0)
		{
			bufferIndex = STEPPER_maxIndex - STEPPER_tick_count;
			sinValue = STEPPER_sinBuffer[bufferIndex];
			cosValue = STEPPER_cosBuffer[bufferIndex];

			if (bufferIndex == 0)
			{
				STEPPER_tick_count = 0;
				STEPPER_stepToDo = STEPPER_stepToDo + 1;
			}
		}
	}

	PWM_Set_Value(sinValue, cosValue);
}

int32_t STEPPER_GetStepToDo()
{
	return STEPPER_stepToDo;
}


void STEPPER_SetStepToDo(int32_t nstep)
{
	STEPPER_stepToDo = nstep;
}


