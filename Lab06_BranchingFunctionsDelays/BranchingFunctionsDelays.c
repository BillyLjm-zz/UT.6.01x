// BranchingFunctionsDelays.c Lab 6
// Runs on LM4F120/TM4C123
// Use simple programming structures in C to 
// toggle an LED while a button is pressed and 
// turn the LED on when the button is released.  
// This lab will use the hardware already built into the LaunchPad.
// Daniel Valvano, Jonathan Valvano
// January 8, 2014

// built-in connection: PF0 connected to negative logic momentary switch, SW2
// built-in connection: PF1 connected to red LED
// built-in connection: PF2 connected to blue LED
// built-in connection: PF3 connected to green LED
// built-in connection: PF4 connected to negative logic momentary switch, SW1

#include "TExaS.h"

#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC2_GPIOF      0x00000020  // port F Clock Gating Control

// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

// function to delay for 100ms
void Delay100ms(){
  unsigned long i;
    i = 1333333; 
    while(i > 0)
      i = i - 1;
}

int main(void){ unsigned long volatile delay;
  TExaS_Init(SW_PIN_PF4, LED_PIN_PF2);  // activate grader and set system clock to 80 MHz
	
  // initialization goes here
  // written by Billy.Ljm
	SYSCTL_RCGC2_R |= 0x00000020;     // activate clock for Port F
  delay = SYSCTL_RCGC2_R;           // allow time for clock to start
	GPIO_PORTF_AMSEL_R = 0x00;        // disable analog on PF
	GPIO_PORTF_PCTL_R = 0x00000000;   // PCTL GPIO on PF
	GPIO_PORTF_DIR_R = 0x04;          // PF2 out & PF4 in (rest are in)
	GPIO_PORTF_AFSEL_R = 0x00;        // disable alt funct on PF
	GPIO_PORTF_DEN_R = 0x14;          // enable digital I/O on PF4 & PF2
	GPIO_PORTF_PUR_R = 0x10;          // enable pull-up on PF4
	GPIO_PORTF_DATA_R = 0x14;					// turn LED on
	
  EnableInterrupts();           // enable interrupts for the grader
  
	while(1){
	//written by Billy.Ljm
    Delay100ms(); // delay for 100ms
		
		// Toggle LED if PF4 pressed (and PF2 is not pressed)
		if(GPIO_PORTF_DATA_R == 0x05) // Turn LED OFF
			GPIO_PORTF_DATA_R = 0X01;
		else if(GPIO_PORTF_DATA_R == 0x01) // Turn LED ON
			GPIO_PORTF_DATA_R = 0X05;
		
		// Toggle LED if PF4 pressed (and PF2 is pressed)
		else if(GPIO_PORTF_DATA_R == 0x04) // Turn LED OFF
			GPIO_PORTF_DATA_R = 0X00;
		else if (GPIO_PORTF_DATA_R == 0x00) // Turn LED ON
			GPIO_PORTF_DATA_R = 0X04;
  }
}
