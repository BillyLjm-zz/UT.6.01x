/**********************************************
 * Sound.c
 * UT.6.01x lab 13 
 * Runs on LM4F120 or TM4C123, 
 * 
 * Outputs sine-wave to 4-bit DAC at PB0-PB3
 * 
 * coded by Billy.Ljm
 * 12 April 2014
 **********************************************/
#include "Sound.h"
#include "DAC.h"
#include "..//tm4c123gh6pm.h"

int soundOff = 0; // bool for if button not pressed & sound should be off
int indx = 0; // index for SineWave, for SysTick ISR to use
const int SineWave[64] = // sine wave ranging for 0 to 15 with 256 samples
{8,8,9,10,10,11,12,12,13,13,14,14,14,15,15,15,15,15,15,15,14,14,14,13,13,12,11,11,10,9,
9,8,7,6,6,5,4,4,3,2,2,1,1,1,0,0,0,0,0,0,0,1,1,1,2,2,3,3,4,5,5,6,7,8};



// **************Sound_Init*********************
// Initialize Systick periodic interrupts
// Also calls DAC_Init() to initialize DAC
// Input: none
// Output: none
void Sound_Init(void){
	DAC_Init();		// initialise DAC
	NVIC_ST_CTRL_R = 0;           								// disable SysTick 
  NVIC_ST_CURRENT_R = 0;        								// any write to current clears it
  NVIC_SYS_PRI3_R = NVIC_SYS_PRI3_R&0x00FFFFFF; // priority 0  
	NVIC_ST_CTRL_R = 0x07; // enable SysTic	
}

// **************Sound_Tone*********************
// Change Systick periodic interrupts to start sound output
// Input: interrupt period
//           Units of period are 12.5ns
//           Maximum is 2^24-1
//           Minimum is 256
// Output: none
void Sound_Tone(unsigned long period){
	// this routine sets the RELOAD and starts SysTick	
	NVIC_ST_CTRL_R = 0; // disable SysTick
	indx = 0; // reset sine wave
	NVIC_ST_RELOAD_R = period -1;
	soundOff = 0;
	NVIC_ST_CTRL_R = 0x07; // enable SysTick
}


// **************Sound_Off*********************
// stop outputing to DAC
// Output: none
void Sound_Off(void){
	soundOff = 1;
	DAC_Out(0);
}


// Interrupt service routine
// Executed every 12.5ns*(period)
void SysTick_Handler(void){
	if(!soundOff){
		DAC_Out(SineWave[indx]);
		indx = (indx + 1) % 64;
	}
}
