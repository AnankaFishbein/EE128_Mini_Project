/* ###################################################################
 **     Filename    : main.c
 **     Project     : Lab_6
 **     Processor   : MK64FN1M0VLL12
 **     Version     : Driver 01.01
 **     Compiler    : GNU C Compiler
 **     Date/Time   : 2024-11-05, 14:21, # CodeGen: 0
 **     Abstract    :
 **         Main module.
 **         This module contains user's application code.
 **     Settings    :
 **     Contents    :
 **         No public methods
 **
 ** ###################################################################*/
/*!
 ** @file main.c
 ** @version 01.01
 ** @brief
 **         Main module.
 **         This module contains user's application code.
 */
/*!
 **  @addtogroup main_module main module documentation
 **  @{
 */
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "Pins1.h"
#include "FX1.h"
#include "MCUC1.h"
#include "GI2C1.h"
#include "WAIT1.h"
#include "CI2C1.h"
#include "CsIO1.h"
#include "IO1.h"
#include "SM1.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "PDD_Includes.h"
#include "Init_Config.h"
/* User includes (#include below this line is not maintained by Processor Expert) */
#include "MK64F12.h"

/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
unsigned char write[512];
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
	/* Write your local variable definition here */

	/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
	PE_low_level_init();
	/*** End of Processor Expert internal initialization.                    ***/

	/* Write your code here */
	uint32_t delay;
	//uint8_t ret, who;

	//ADC Photo Variables
	unsigned int light_value;
	const unsigned int day_limit = 3000;
	const unsigned int night_limit = 200;
	unsigned int timeOfDay; //0 for night, 1 for day, 2 for unsure
	unsigned int lastTime; //to hold until time is toggled

	//Keypad Variables
	unsigned int voltIn;
	int keyIn;
	//const unsigned int index[7] = {38000, 33000, 35000, 36000, 30000, 28000, 39000};
	//const unsigned int idleIn = 65000;
	//const int keys[7] = {1, 2, 3, 4, 5, 6, 7};
	//const int range = 1000;
	int tempKey;

	int pwd[2] = {1, 2}; //by default

	//Arduino transfer Varibales
	int len;
	LDD_TDeviceData *SM1_DeviceData;
	SM1_DeviceData = SM1_Init(NULL);

//	//Stepper motor variables
//	int isCorrect = 0;                           // Boolean flag
//	int input[4];                      // Input string (user input simulation)


	//Arduino transfer init
	FX1_Init();

	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK; /* Enable Port B Clock Gate Control */
	PORTB_PCR2 = 0x100; //Port B, pin 2, for GPIO
	GPIOB_PDDR |= ((unsigned long)1 << 2); /*Setting the bit 2 of the port B as Output*/

	// ADC init (for both Keypad and Photo)
	SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK; // 0x8000000u; Enable ADC0 Clock
	ADC0_CFG1 = 0x0C; // 16bits ADC; Bus Clock
	ADC0_SC1A = 0x1F; // Disable the module, ADCH = 11111

//	// Stepper motor init
//	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK; // Enable Port C Clock Gate Control
//	PORTC_GPCLR = 0x00FF0100;          // Initialize GPIO Port C
//	GPIOC_PDDR = 0x000000FF;           // Set PTc0-PTc7 as outputs



	for(;;) {
		// get WHO AM I values
		//  		if (FX1_WhoAmI(&who)!=ERR_OK) {
		//  			return ERR_FAILED;
		//  		}


		//Photo section
		light_value = (unsigned int)ADC_read16bPhoto(); // read lightness from photo from ADC0_DP0
		for(delay = 0; delay < 300000; delay++); //delay
		//printf("photo reads: %d\n", light_value);

//		//SPI Transfer to Arduino
//		timeOfDay = getTime(light_value, day_limit); //day is 1, night is 0
//		len = sprintf(write, "T%d\n",timeOfDay);
//		GPIOB_PDOR |= ((unsigned long)1 << 2); //set pin 2 to high
//		SM1_SendBlock(SM1_DeviceData, &write, len);
//		for(delay = 0; delay < 300000; delay++); //delay
//		GPIOB_PDOR &= ~((unsigned long)1 << 2); //set pin 2 to low


		//Keypad section
		//if(timeOfDay == 1){
			voltIn = ADC_read16bKey();
			printf("voltIn reads: %d\n", voltIn);
			for(delay = 0; delay < 300000; delay++); //delay

			tempKey = readKey(voltIn);
			printf("key is: %d", tempKey);
//			if(tempKey != 0 && tempKey != 9){
//				//send begin to enter signal to arduino
//				len = sprintf(write, "U%d\n", tempKey);
//				GPIOB_PDOR |= ((unsigned long)1 << 2); //set pin 2 to high
//				SM1_SendBlock(SM1_DeviceData, &write, len);
//				for(delay = 0; delay < 300000; delay++); //delay
//				GPIOB_PDOR &= ~((unsigned long)1 << 2); //set pin 2 to low
//
//				for(int i = 0; i < 2; i++){
//					voltIn = ADC_read16bKey();
//					//printf("voltIn reads: %d\n", voltIn);
//					for(delay = 0; delay < 500000; delay++); //delay
//					tempKey = readKey(voltIn);
//					if(tempKey != pwd[i]){
//						//send wrong pwd meg to Arduino
//						len = sprintf(write, "W%d\n", tempKey);
//						GPIOB_PDOR |= ((unsigned long)1 << 2); //set pin 2 to high
//						SM1_SendBlock(SM1_DeviceData, &write, len);
//						for(delay = 0; delay < 300000; delay++); //delay
//						GPIOB_PDOR &= ~((unsigned long)1 << 2); //set pin 2 to low
//						break;//break out of pwd entering loop
//					}else{
//						//correct pwd, print key
//						len = sprintf(write, "R%d\n", tempKey);
//						GPIOB_PDOR |= ((unsigned long)1 << 2); //set pin 2 to high
//						SM1_SendBlock(SM1_DeviceData, &write, len);
//						for(delay = 0; delay < 300000; delay++); //delay
//						GPIOB_PDOR &= ~((unsigned long)1 << 2); //set pin 2 to low
//					}
//				}
//				//finished pwd entering, send ending msg to Arduino
//				len = sprintf(write, "E%d\n", tempKey);
//				GPIOB_PDOR |= ((unsigned long)1 << 2); //set pin 2 to high
//				SM1_SendBlock(SM1_DeviceData, &write, len);
//				for(delay = 0; delay < 300000; delay++); //delay
//				GPIOB_PDOR &= ~((unsigned long)1 << 2); //set pin 2 to low
//
//			}
	//	}

		//stepper motor starts here
		// Simulated user input
		//strcpy(input, "1234"); // Replace this with actual input gathering logic(after keypad is intergrated)

		// Check password
		//isCorrect = checkPassword(input);
		isCorrect = timeOfDay;

//		//stepper rotate section
//		if (isCorrect == 0) {
//			// Rotate backward (50 steps)
//			for (int steps = 0; steps < 50; steps++) {
//				rotateBackward();
//			}
//			for(delay = 0; delay < 5000000; delay++); //delay
//
//			// Return to original position (50 steps forward)
//			for (int steps = 0; steps < 50; steps++) {
//				rotateForward();
//			}
//
//			isCorrect = 0; // Reset flag
//
//
//		}

	}

	/*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
 ** @}
 */
/*
 ** ###################################################################
 **
 **     This file was created by Processor Expert 10.5 [05.21]
 **     for the Freescale Kinetis series of microcontrollers.
 **
 ** ###################################################################
 */
