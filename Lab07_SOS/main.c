// 0.Documentation Section 
// Lab7_SOS, main.c

// Runs on LM4F120 or TM4C123 LaunchPad
// Input from PF4(SW1) and PF0(SW2), output to PF1 and PF3 (Yellow LED)
// Pressing both SW1 & SW2 starts SOS (Yellow LED flashes SOS).
//    S: Toggle light 3 times with 1/2 sec gap between ON....1/2sec....OFF
//    O: Toggle light 3 times with 2 sec gap between ON....2sec....OFF
//    S: Toggle light 3 times with 1/2 sec gap between ON....1/2sec....OFF
//    4 second delay between SOS
// Releasing either SW1 or SW2 stops SOS

// Assignemnt made by: Daniel Valvano, Jonathan Valvano and Ramesh Yerraballi

// 1. Pre-processor Directives Section
#include "TExaS.h"

// Constant declarations to access port registers using 
// symbolic names instead of addresses
#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_LOCK_R       (*((volatile unsigned long *)0x40025520))
#define GPIO_PORTF_CR_R         (*((volatile unsigned long *)0x40025524))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))

// 2. Declarations Section
//   Global Variables
unsigned long SW1; // input from PF4
unsigned long SW2; // input from PF0
//   Function Prototypes
void PortF_Init(void);
void FlashSOS(void);
void delay(unsigned long halfsecs);
void EnableInterrupts(void);  // Enable interrupts

// 3. Subroutines Section
// MAIN: Mandatory for a C Program to be executable
int main(void){
  TExaS_Init(SW_PIN_PF40, LED_PIN_PF321);  // activate grader and set system clock to 80 MHz
  PortF_Init(); // Init port PF4 PF2 PF0    
  EnableInterrupts();           // enable interrupts for the grader
  while(1){
		SW1 = GPIO_PORTF_DATA_R&0x10; // PF4 into SW1
		SW2 = GPIO_PORTF_DATA_R&0x01; // PF0 into SW2
    // Flash SOS while both SW1 & SW2 are pressed  
		if(SW1 == 0x00 && SW2 == 0x00)
			FlashSOS();
  }
}
// Subroutine to initialize port F pins for input and output
// PF4 is input SW1 and PF2 is output Blue LED
// Inputs: None
// Outputs: None
// Notes: ...
void PortF_Init(void){ 
  // written by Billy.Ljm
  volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000020;     // 1) F clock
  delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock PortF PF0  
  GPIO_PORTF_CR_R |= 0x1F;           // allow changes to PF4-0       
  GPIO_PORTF_AMSEL_R &= 0x00;        // 3) disable analog function
  GPIO_PORTF_PCTL_R &= 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTF_DIR_R &= ~0x11;          // 5.1) PF4,PF0 input, 
  GPIO_PORTF_DIR_R |= 0x0E;          // 5.2) PF1,PF2,PF3 output  
  GPIO_PORTF_AFSEL_R &= 0x00;        // 6) no alternate function
  GPIO_PORTF_PUR_R |= 0x11;          // enable pullup resistors on PF4,PF0       
  GPIO_PORTF_DEN_R |= 0x1F;          // 7) enable digital pins PF4-PF0     
}
// Color    LED(s) PortF
// dark     ---    0
// red      R--    0x02
// blue     --B    0x04
// green    -G-    0x08
// yellow   RG-    0x0A
// sky blue -GB    0x0C
// white    RGB    0x0E

// Subroutine to Flash a yellow SOS once
// PF1 & PF3 is yellow: SOS 
//    S: Toggle light 3 times with 1/2 sec gap between ON....1/2sec....OFF
//    O: Toggle light 3 times with 2 sec gap between ON....2sec....OFF
//    S: Toggle light 3 times with 1/2 sec gap between ON....1/2sec....OFF
// Inputs: None
// Outputs: None
// Notes: ...
void FlashSOS(void){
  // Written by Billy.Ljm
  //S
  GPIO_PORTF_DATA_R |= 0x0A;  delay(1);
  GPIO_PORTF_DATA_R &= ~0x0A; delay(1);
  GPIO_PORTF_DATA_R |= 0x0A;  delay(1);
  GPIO_PORTF_DATA_R &= ~0x0A; delay(1);
  GPIO_PORTF_DATA_R |= 0x0A;  delay(1);
  GPIO_PORTF_DATA_R &= ~0x0A; delay(1);
  //O
  GPIO_PORTF_DATA_R |= 0x0A; delay(4);
  GPIO_PORTF_DATA_R &= ~0x0A;delay(4);
  GPIO_PORTF_DATA_R |= 0x0A; delay(4);
  GPIO_PORTF_DATA_R &= ~0x0A;delay(4);
  GPIO_PORTF_DATA_R |= 0x0A; delay(4);
  GPIO_PORTF_DATA_R &= ~0x0A;delay(4);
  //S
  GPIO_PORTF_DATA_R |= 0x0A; delay(1);
  GPIO_PORTF_DATA_R &= ~0x0A;delay(1);
  GPIO_PORTF_DATA_R |= 0x0A; delay(1);
  GPIO_PORTF_DATA_R &= ~0x0A;delay(1);
  GPIO_PORTF_DATA_R |= 0x0A; delay(1);
  GPIO_PORTF_DATA_R &= ~0x0A;delay(1);
  delay(8); // Delay for 5 secs in between flashes 

}

// Subroutine to delay in units of half seconds
// Inputs: Number of half seconds to delay
// Outputs: None
// simple delay function
// which delays time*500 milliseconds
// assuming 80 MHz clock
void delay(unsigned long time){
  unsigned long i;
  while(time > 0){
    i = 6666665;
    while(i > 0){
      i = i - 1;
    }
    time = time - 1;
  }
}

