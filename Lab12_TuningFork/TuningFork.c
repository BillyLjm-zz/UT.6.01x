// TuningFork.c Lab 12
// Runs on LM4F120/TM4C123
// Use SysTick interrupts to create a squarewave at 440Hz.  
// There is a positive logic switch connected to PA3.
// There is an output on PA2. The output is 
//   connected to headphones through a 1k resistor.
// The volume-limiting resistor can be any value from 680 to 2000 ohms
// The tone is initially off, when the switch goes from
// not touched to touched, the tone toggles on/off.
//                   |---------|               |---------|     
// Switch   ---------|         |---------------|         |------
//
//                    |-| |-| |-| |-| |-| |-| |-|
// Tone     ----------| |-| |-| |-| |-| |-| |-| |---------------
//
// Daniel Valvano, Jonathan Valvano
// March 8, 2014

/* This example accompanies the book
   "Embedded Systems: Introduction to ARM Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2013
   "Embedded Systems: Real Time Interfacing to ARM Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2013

 Copyright 2014 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */


#include "TExaS.h"
#include "..//tm4c123gh6pm.h"

#define PA2 (*(volatile unsigned long *)0x40004010)
#define PA3 (*(volatile unsigned long *)0x40004020)
#define pressed 0x08 // for button at PF3
#define noisy 0 // for current state

int currState;
int prevPA3;

// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void WaitForInterrupt(void);  // low power mode

// input from PA3, output from PA2, SysTick interrupts
void Sound_Init(void){ 
	// Port A init
	unsigned long volatile delay;
	SYSCTL_RCGC2_R |= 0x00000001; // activate port A
  delay = SYSCTL_RCGC2_R;
  GPIO_PORTA_AMSEL_R &= ~0x20;      // no analog
  GPIO_PORTA_PCTL_R &= ~0x0000FF00; // regular function
	GPIO_PORTA_DIR_R |= 0x04;     // make PA2 out
	GPIO_PORTA_DIR_R &= ~0X08;		// make PA3 in
  GPIO_PORTA_DR8R_R |= 0x20;    // can drive up to 8mA out
  GPIO_PORTA_AFSEL_R &= ~0x0C;  // disable alt funct on PA2,PA3
  GPIO_PORTA_DEN_R |= 0x0C;     // enable digital I/O on PA2,PA3
  
	// Systick Init
	NVIC_ST_CTRL_R = 0;           // disable SysTick during setup
  NVIC_ST_RELOAD_R = 90908;     // reload value for ~880Hz (assuming 80MHz PLL)
  NVIC_ST_CURRENT_R = 0;        // any write to current clears it
  NVIC_SYS_PRI3_R = NVIC_SYS_PRI3_R&0x00FFFFFF; // priority 0               
  NVIC_ST_CTRL_R = 0x07;  // enable with core clock and interrupts
}

// called at 880 Hz
void SysTick_Handler(void){
	if(prevPA3 != pressed && PA3 == pressed)
		currState ^= 0x01; // toggle current state
	if(currState == noisy)
		PA2 ^= 0x04;
	else
		PA2 = 0;
	prevPA3 = PA3;
}

int main(void){// activate grader and set system clock to 80 MHz
  TExaS_Init(SW_PIN_PA3, HEADPHONE_PIN_PA2,ScopeOn); 
  Sound_Init();
	currState = 1; // not noisy
	prevPA3 = 1; // not pressed
  EnableInterrupts();   // enable after all initialization are done
  while(1){
    // main program is free to perform other tasks
    // do not use WaitForInterrupt() here, it may cause the TExaS to crash
  }
}
