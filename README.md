Author: Gibellini Matteo

A simple stepper motor control with NucleoF401 and L298N

Connections:
ENA=>+5V
ENB=>+5V
IN1=>PA6
IN2=>PA7
IN3=>PC8
IN4=>PC9
OUT1/2=>Motor coil 1
OUT3/4=>Motor coil 2

Download last STM32CubeIDE

Change parameters in stepper.c:

#define STEPPER_VIN 24.0f   //[V] supply voltage
#define STEPPER_SPR 200.0f  //Step per round of the motor
						    //resolution is reduced by 4
							//so if SPR = 200 only 50 step to make one turn of the shaft
#define STEPPER_RPS 5.0f    //Round per second (Speed) 0.5<->5
#define STEPPER_VPEAK 16.0f //[V] Peak voltage in moving condition
							//it depends on speed, keep RMS current as low as possible (under 1.0A)
#define STEPPER_VOFF 4.0f   //[V] Max voltage in still condition
							//keep medium current as low as possible (under 1.0A)

