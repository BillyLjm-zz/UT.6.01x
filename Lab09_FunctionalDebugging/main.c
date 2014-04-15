// ***** 0. Documentation Section *****
// main.c for Lab 9
// Runs on LM4F120/TM4C123
// In this lab we are learning functional debugging by dumping
//   recorded I/O data into a buffer
// February 10, 2014

// Lab 9
//      Jon Valvano and Ramesh Yerraballi

/***************************
 * Completed by Billy.Ljm  *
 *    on 9 March 2014      *
 ***************************/

// ***** 1. Pre-processor Directives Section *****
#include "TExaS.h"
#include "tm4c123gh6pm.h"
#include <cstdio>

// ***** 2. Global Declarations Section *****

// FUNCTION PROTOTYPES: Each subroutine defined
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

// ***** 3. Subroutines Section *****

/* 
This Lab9 starter project is the same as C9_Debugging example but 
includes the connections to the Lab9 grader. You will make three changes. 
First, make the LED flash at 10 Hz. In other words, make it turn on for 0.05 seconds, 
and then turn off for 0.05 seconds. 
Second, make the LED flash if either switch SW1 or SW2 are pressed 
(this means either PF4 or PF0 is 0). 
Third, record PortF bits 4,1,0 every time the input changes or the output changes. 
For example, if your system detects a change in either PF4 or PF0 input, 
record PortF bits 4,1,0. If your system causes a change in PF1, record PortF bits 4,1,0. 

If both PF4 and PF0 switch are not pressed, the PF1 output should be low.  
If either PF4 or PF0 switches is pressed, the output toggles at 10 Hz (±10%). 
Information collected in the Data array matches the I/O on PortF.
50 data points are collected only on a change in input or a change in output.
(i.e., no adjacent elements in the array are equal).
*/

// ***** Given helper functions *****

void PortF_Init(void){ volatile unsigned long delay;
	// Initialises port F
  SYSCTL_RCGC2_R |= 0x00000020;     // 1) activate clock for Port F
  delay = SYSCTL_RCGC2_R;           // allow time for clock to start
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
  // only PF0 needs to be unlocked, other bits can't be locked
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog on PF
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) PCTL GPIO on PF4-0
  GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 in, PF3-1 out
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) disable alt funct on PF7-0
  GPIO_PORTF_PUR_R = 0x11;          // enable pull-up on PF0 and PF4
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital I/O on PF4-0
}

void SysTick_Init(void){
	// Initialize SysTick with busy wait running at bus clock.
  NVIC_ST_CTRL_R = 0;                   // disable SysTick during setup
  NVIC_ST_RELOAD_R = 0x00FFFFFF;        // maximum reload value
  NVIC_ST_CURRENT_R = 0;                // any write to current clears it             
  NVIC_ST_CTRL_R = 0x00000005;          // enable SysTick with core clock
}

void Delay(void){
	// Delays for 0.05sec
	unsigned long volatile time = 80000; // 0.05sec
  while(time)
   time--;
}

// ****** Data dump arrays *****
unsigned long Time[50]; // first data point is wrong, the other 49 will be correct
unsigned long Data[50]; // you must leave thr Data array defined exactly as it is

// ***** Main function *****
int main(void){  
	// Variables
	unsigned long i = 0; // array index for Time & Data
	unsigned long sws; // current SW1 & SW2
	unsigned long data; // current PF0,PF1 & PF4
	unsigned long prevdata; // previous PF0, PF1 & PF4
	unsigned long last = NVIC_ST_CURRENT_R; // time of last data dump
	unsigned long now; // current time

	// Initialisation
	TExaS_Init(SW_PIN_PF40, LED_PIN_PF1);  // activate grader and set system clock to 80 MHz
  PortF_Init();   // initialize PF1 to output
  SysTick_Init(); // initialize SysTick, runs at 16 MHz
  EnableInterrupts();           // enable interrupts for the grader
	prevdata = GPIO_PORTF_DATA_R & 0x13;
	
  while(1){
		// Main function
			// Flicker LED is either SW1 pressed
			// Else turn LED off
		sws = GPIO_PORTF_DATA_R & 0x11;
		if(sws == 0x011)
			GPIO_PORTF_DATA_R &= ~0x02;
		else
			GPIO_PORTF_DATA_R ^= 0x02;
	
		// Data dump
			// Record PF0,PF1 & PF4 if any of them change
			// Only 50 earliest dumps
		data = GPIO_PORTF_DATA_R & 0x13;
    if(i<50 && data != prevdata){
      now = NVIC_ST_CURRENT_R;
      Time[i] = (last-now)&0x00FFFFFF;  // 24-bit time difference
      Data[i] = data;
      last = now;
			prevdata = data;
      i++;
		}
		
		// Flicker at 10Hz
		Delay(); 
  }
}
