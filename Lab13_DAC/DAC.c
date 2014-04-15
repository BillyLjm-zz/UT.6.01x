/**********************************************
 * DAC.c
 * UT.6.01x lab 13 
 * Runs on LM4F120 or TM4C123, 
 * 
 * Implementation of the 4-bit DAC on PB3-PB0
 * 
 * coded by Billy.Ljm
 * 12 April 2014
 **********************************************/

#include "DAC.h"
#include "..//tm4c123gh6pm.h"

// **************DAC_Init*********************
// Initialize 4-bit DAC at PB0-PB3
// Input: none
// Output: none
void DAC_Init(void){
	unsigned long volatile delay;
	SYSCTL_RCGC2_R |= 0x02; 					// activate port B
  delay = SYSCTL_RCGC2_R;
  GPIO_PORTB_DIR_R |= 0X0F;     		// make PB0-PB5 out
	GPIO_PORTB_AFSEL_R &= ~0X0F;  		// disable alt funct on PB0-PB5
	GPIO_PORTB_DEN_R |= 0X0F;     		// enable digital I/O on PB0-PB5
	GPIO_PORTB_AMSEL_R &= ~0x20;      // no analog
  GPIO_PORTB_PCTL_R = 0x00; 			// regular function
	GPIO_PORTB_DR8R_R |= 0x0F;				// drive up to 8mA out
}


// **************DAC_Out*********************
// output to DAC
// Input: 4-bit data, 0 to 15
// Output: none
void DAC_Out(unsigned long data){
  GPIO_PORTB_DATA_R = data; 
}
