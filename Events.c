/* ###################################################################
**     Filename    : Events.c
**     Project     : Lab_6
**     Processor   : MK64FN1M0VLL12
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2024-11-05, 14:21, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Contents    :
**         Cpu_OnNMI - void Cpu_OnNMI(void);
**
** ###################################################################*/
/*!
** @file Events.c
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         
/* MODULE Events */

#include "Cpu.h"
#include "Events.h"
#include "Init_Config.h"
#include "PDD_Includes.h"

#ifdef __cplusplus
extern "C" {
#endif 


/* User includes (#include below this line is not maintained by Processor Expert) */

/*
** ===================================================================
**     Event       :  Cpu_OnNMI (module Events)
**
**     Component   :  Cpu [MK64FN1M0LL12]
*/
/*!
**     @brief
**         This event is called when the Non maskable interrupt had
**         occurred. This event is automatically enabled when the [NMI
**         interrupt] property is set to 'Enabled'.
*/
/* ===================================================================*/
void Cpu_OnNMI(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  SM1_OnBlockSent (module Events)
**
**     Component   :  SM1 [SPIMaster_LDD]
*/
/*!
**     @brief
**         This event is called after the last character from the
**         output buffer is moved to the transmitter. This event is
**         available only if the SendBlock method is enabled.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. The pointer is passed
**                           as the parameter of Init method. 
*/
/* ===================================================================*/
void SM1_OnBlockSent(LDD_TUserData *UserDataPtr)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  SM1_OnBlockReceived (module Events)
**
**     Component   :  SM1 [SPIMaster_LDD]
*/
/*!
**     @brief
**         This event is called when the requested number of data is
**         moved to the input buffer. This method is available only if
**         the ReceiveBlock method is enabled.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. The pointer is passed
**                           as the parameter of Init method. 
*/
/* ===================================================================*/
void SM1_OnBlockReceived(LDD_TUserData *UserDataPtr)
{
  /* Write your code here ... */
}

unsigned short ADC_read16bPhoto(void)
{
	ADC0_SC1A = 0x0; //Write to SC1A to start conversion from ADC_DP0
	while(ADC0_SC2 & ADC_SC2_ADACT_MASK); // Conversion in progress
	while(!(ADC0_SC1A & ADC_SC1_COCO_MASK)); // Until conversion complete
	return ADC0_RA;
}

unsigned short ADC_read16bKey(void)
{
	ADC0_SC1A = 0x1; //Write to SC1A to start conversion from ADC_DP1
	while(ADC0_SC2 & ADC_SC2_ADACT_MASK); // Conversion in progress
	while(!(ADC0_SC1A & ADC_SC1_COCO_MASK)); // Until conversion complete
	return ADC0_RA;
}

int readKey(unsigned int voltIn){
	int tempKey;
	if(voltIn >= 64000 && voltIn <= 66000){
		tempKey = 0; //idle
	}else if(voltIn >= 37000 && voltIn <= 42000){
		tempKey = 1;
	}else if (voltIn >= 32000 && voltIn <= 36000){
		tempKey = 2;
	}else if(voltIn >= 23000 && voltIn <= 28000){
		tempKey = 5;
	}else{
		tempKey = 9; //noise
	}
	return tempKey;
}

unsigned int getTime(unsigned int light_value, unsigned int day_limit){
	unsigned int timeOfDay = 2;
	if(light_value > day_limit){
				timeOfDay = 1;
				printf("DAY\n");
			}
			else /*if(light_value < night_limit)*/{
				timeOfDay = 0;
				printf("Night\n");
			}
			//printf("photo reads: %d\n", light_value);
	return timeOfDay;
}

void rotateForward(void) {
	uint32_t delay;
    GPIOC_PDOR = 0x36; // Step 1: A1, B1 HIGH
    for(delay = 0; delay < 25000; delay++); //delay
    GPIOC_PDOR = 0x3A; // Step 2: A2, B1 HIGH
    for(delay = 0; delay < 25000; delay++); //delay
    GPIOC_PDOR = 0x39; // Step 3: A2, B2 HIGH
    for(delay = 0; delay < 25000; delay++); //delay
    GPIOC_PDOR = 0x35; // Step 4: A1, B2 HIGH
    for(delay = 0; delay < 25000; delay++); //delay
}

void rotateBackward(void) {
	uint32_t delay;
    GPIOC_PDOR = 0x35; // Step 4: A1, B2 HIGH
    for(delay = 0; delay < 25000; delay++); //delay
    GPIOC_PDOR = 0x39; // Step 3: A2, B2 HIGH
    for(delay = 0; delay < 25000; delay++); //delay
    GPIOC_PDOR = 0x3A; // Step 2: A2, B1 HIGH
    for(delay = 0; delay < 25000; delay++); //delay
    GPIOC_PDOR = 0x36; // Step 1: A1, B1 HIGH
    for(delay = 0; delay < 25000; delay++); //delay
}

//// Password checking function
//int checkPassword(const int input) {
//    int password[4] = [1, 2, 3, 4]; // Set your password here
//    if (input[]== password[]) {
//        return 1; // Password is correct
//    }
//    return 0; // Password is incorrect
//}



/* END Events */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

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
