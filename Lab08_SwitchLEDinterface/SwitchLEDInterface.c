// ***** 0. Documentation Section *****
// SwitchLEDInterface.c for Lab 8
// Runs on LM4F120/TM4C123
// Use simple programming structures in C to:
// 1. Flash LED at 5Hz if switch pressed
// 2. Turn LED on (no flashing) if switch released
// This lab requires external hardware
// to be wired to the LaunchPad using the prototyping board.
// January 11, 2014

// Lab 8
//      Jon Valvano and Ramesh Yerraballi
//      November 21, 2013

// ***** 1. Pre-processor Directives Section *****
#include "TExaS.h"
#include "tm4c123gh6pm.h"

// ***** 2. Global Declarations Section *****

// FUNCTION PROTOTYPES: Each subroutine defined
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

/* ***** 3. Subroutines Section *****
 * PE0: +ve logic switch
 * PE1: +ve logic LED
 *************************************/

// Define port E addresses
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define GPIO_PORTE_DIR_R        (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_AFSEL_R      (*((volatile unsigned long *)0x40024420))
#define GPIO_PORTE_PUR_R 				(*((volatile unsigned long *)0x40024510))
#define GPIO_PORTE_DEN_R        (*((volatile unsigned long *)0x4002451C))
#define GPIO_PORTE_AMSEL_R      (*((volatile unsigned long *)0x40024528))
#define GPIO_PORTE_PCTL_R       (*((volatile unsigned long *)0x4002452C))
#define PE0											(*((volatile unsigned long *)0x40024004))
#define PE1											(*((volatile unsigned long *)0x40024008))

void port_init(void){
	// Initialise port e
	// written by Billy.Ljm
	unsigned long volatile delay;
	SYSCTL_RCGC2_R |= 0x10;           // Port E clock
  delay = SYSCTL_RCGC2_R;           // wait 3-5 bus cycles
  GPIO_PORTE_DIR_R |= 0x02;         // PE1 output
  GPIO_PORTE_DIR_R &= ~0x01;        // PE0 input 
  GPIO_PORTE_AFSEL_R &= ~0x03;      // not alternative
  GPIO_PORTE_AMSEL_R &= ~0x03;      // no analog
  GPIO_PORTE_PCTL_R &= 0x00000000; 	// clear PCTL
	GPIO_PORTE_PUR_R &= 0x00;					// no pull-up
  GPIO_PORTE_DEN_R |= 0x03;         // enable PE0, PE1
}

void Delay100ms(void){
	// function to delay for 100ms
  unsigned long i;
    i = 1333333; 
    while(i > 0)
      i = i - 1;
}

int main(void){ 
	// written by Billy.Ljm
  TExaS_Init(SW_PIN_PE0, LED_PIN_PE1); // activate grader and set system clock to 80 MHz
  EnableInterrupts(); // enable interrupts for the grader
  port_init(); // initialise porrt E
	PE1 = 0x02;	// turn LED on
	
	while(1){
		if (PE0 == 0x01) // switch pressd, toggle LED
			PE1 ^= 0x02;
		else // switch released, turn on LED
			PE1 = 0x02;
		Delay100ms(); // delay to read
  }
}
