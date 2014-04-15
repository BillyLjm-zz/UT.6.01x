/**********************************************
 * Piano.c
 * UT.6.01x lab 13 
 * Runs on LM4F120 or TM4C123, 
 * 
 * Implementation of 4 piano keys on PE0-PE3
 * 
 * coded by Billy.Ljm
 * 12 April 2014
 **********************************************/

#include "Piano.h"
#include "..//tm4c123gh6pm.h"


// **************Piano_Init*********************
// Initialize piano key inputs at PE0 - PE3
// Input: none
// Output: none
void Piano_Init(void){ 
  unsigned long volatile delay;
	SYSCTL_RCGC2_R |= 0x10; 					// activate port E
  delay = SYSCTL_RCGC2_R;
  GPIO_PORTE_DIR_R &= ~0x0F;     		// make PE0-PE3 in
	GPIO_PORTE_AFSEL_R &= ~0x0F;  		// disable alt funct on PE0-PE3
	GPIO_PORTE_DEN_R |= 0x0F;     		// enable digital I/O on PE0-PE3
	GPIO_PORTE_PUR_R &= ~0x0F;				// disable pull-up
	GPIO_PORTE_AMSEL_R &= ~0x20;      // no analog
  GPIO_PORTE_PCTL_R &= ~0x0F; 			// regular function
}

// **************Piano_In*********************
// Input from piano key inputs
// Input: none 
// Output: 0 to 15 depending on keys
// 0x01 is key 0 pressed, 0x02 is key 1 pressed,
// 0x04 is key 2 pressed, 0x08 is key 3 pressed
unsigned long Piano_In(void){
  return GPIO_PORTE_DATA_R & 0x0F;
}
