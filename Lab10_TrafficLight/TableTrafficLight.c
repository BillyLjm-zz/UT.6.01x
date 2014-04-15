// ***** 0. Documentation Section *****
// TableTrafficLight.c for Lab 10
// Runs on LM4F120/TM4C123
// Index implementation of a Moore finite state machine to operate a traffic light.  
// Daniel Valvano, Jonathan Valvano
// November 7, 2013

// Completed by Billy.Ljm
// 19 March 2014

// Southwards green,yellow,red light connected to PB0,PB1,PB2
// Westwards green,yellow,red light connected to PB3,PB4,PB5
// Walk,stop light connected to PF3,PF1
// Detectors for westwards,southwards,pedestrian connected to PE0,PE1,PE2

// ***** 1. Pre-processor Directives Section *****
#include "TExaS.h"
#include "tm4c123gh6pm.h"

// ***** 2. Global Declarations Section *****
#define NVIC_ST_CTRL_R      (*((volatile unsigned long *)0xE000E010))
#define NVIC_ST_RELOAD_R    (*((volatile unsigned long *)0xE000E014))
#define NVIC_ST_CURRENT_R   (*((volatile unsigned long *)0xE000E018))
#define TRAFFIC_LIGHTS			(*((volatile unsigned long *)0x400050FC))
#define PEDESTRIAN_LIGHTS		(*((volatile unsigned long *)0x40025028))
#define SENSORS							(*((volatile unsigned long *)0x4002401C))

// FUNCTION PROTOTYPES: Each subroutine defined
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

// ***** 3. Subroutines Section *****
void ports_Init(void){
	// Written by Billy.Ljm
	// Initialise port B,E,F specifically for this program
	unsigned long volatile delay;
	SYSCTL_RCGC2_R |= 0x32; // activate clock for Port B,E,F
  delay = SYSCTL_RCGC2_R; // allow time for clock to start
	
	// Port B
  GPIO_PORTB_LOCK_R = 0x4C4F434B;   // unlock port
  GPIO_PORTB_CR_R = 0x3F;           // allow changes to PB5-0
	GPIO_PORTB_PCTL_R = 0x00000000;   // clear PCTL
  GPIO_PORTB_AMSEL_R &= ~0x3F;      // disable analog on PB5-0
  GPIO_PORTB_AFSEL_R &= ~0x3F;      // disable alt funct on PB5-0
  GPIO_PORTB_DEN_R |= 0x3F;         // enable digital I/O on PB5-0
	GPIO_PORTB_DIR_R |= 0x3F;         // PB5-0 outputs
	
	// Port E
  GPIO_PORTE_LOCK_R = 0x4C4F434B;   // unlock port
  GPIO_PORTE_CR_R = 0x07;           // allow changes to PE2-0
	GPIO_PORTE_PCTL_R = 0x00000000;   // clear PCTL
  GPIO_PORTE_AMSEL_R &= ~0x07;      // disable analog on PE2-0
  GPIO_PORTE_AFSEL_R &= ~0x07;      // disable alt funct on PE2-0
  GPIO_PORTE_PUR_R &= ~0x07;        // disableb pull-up on PE2-0
  GPIO_PORTE_DEN_R |= 0x07;         // enable digital I/O on PE2-0
	GPIO_PORTE_DIR_R &= ~0x07;        // PE2-0 inputs

	// Port F
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // unlock port
  GPIO_PORTF_CR_R = 0x0A;           // allow changes to PF1 & PF3
	GPIO_PORTF_PCTL_R = 0x00000000;   // clear PCTL
  GPIO_PORTF_AMSEL_R &= ~0x0A;      // disable analog on PF1 & PF3
  GPIO_PORTF_AFSEL_R &= ~0x0A;      // disable alt funct on PF1 & PF3
  GPIO_PORTF_DEN_R |= 0x0A;         // enable digital I/O on PF1 & PF3
	GPIO_PORTF_DIR_R |= 0x0A;         // PF1 & PF3 outputs
}

void SysTick_Init(void){
	// Initalise systick
  NVIC_ST_CTRL_R = 0;               // disable SysTick during setup
  NVIC_ST_CTRL_R = 0x00000005;      // enable SysTick with core clock
}

void SysTick_Wait10ms(){
	// Function to delay for 10ms
  NVIC_ST_RELOAD_R = 8000000 - 1;  // number of counts to wait (80Mhz PLL)
  NVIC_ST_CURRENT_R = 0;       // any value written to CURRENT clears
  while((NVIC_ST_CTRL_R&0x00010000)==0){ // wait for count flag
  }
}
void SysTick_Wait(unsigned long delay){
	// Written by Billy.Ljm
	// Fucntion to delay for integer multiples of 10ms
	unsigned long i;
	for(i=0; i < delay; i++)
		SysTick_Wait10ms();
}

typedef struct Stype{
	// Written by Billy.Ljm
	// struct of a single state in the FSM
	unsigned long TrafficOut; // output for traffic lights (portB)
	unsigned long WalkOut; // output for pedestrian lights (portF)
	unsigned long Time; // delay time (Time * 0.5s)
	unsigned long Next[8]; // next state
} SType;

int main(void){ 
	// Written by Billy.Ljm
	// Variables Used
	unsigned long S = 0; // current state
	SType FSM[11]={
		// States of Finite State Machine (see attached table)
		{0x0C,0x02,20,{0,0,1,1,1,1,1,1}},
		{0x14,0x02,30,{1,0,2,2,4,4,2,2}},
		{0x21,0x02,20,{2,3,2,3,3,3,3,3}},
		{0x22,0x02,30,{3,0,2,0,4,0,4,4}},
		{0x24,0x08,20,{4,5,5,5,4,5,5,5}},
		{0x24,0x00,5,{4,6,6,6,4,6,6,6}},
		{0x24,0x02,5,{4,7,7,7,4,7,7,7}},
		{0x24,0x00,5,{4,8,8,8,4,8,8,8}},
		{0x24,0x02,5,{4,9,9,9,4,9,9,9}},
		{0x24,0x00,5,{4,10,10,10,4,10,10,10}},
		{0x24,0x02,5,{5,0,2,0,4,0,2,0}}
	};
	
	// Initialisation
	TExaS_Init(SW_PIN_PE210, LED_PIN_PB543210); // activate grader and set system clock to 80 MHz
	ports_Init(); // Initialise port B,E,F
	SysTick_Init(); // Initialise systick
  EnableInterrupts();
	
	// Looping through FSM
  while(1){
    TRAFFIC_LIGHTS = FSM[S].TrafficOut;  // set traffic lights
		PEDESTRIAN_LIGHTS = FSM[S].WalkOut; // set pedestrian lights
    SysTick_Wait(FSM[S].Time); // delay
    S = FSM[S].Next[SENSORS];  // next state
  }
}

