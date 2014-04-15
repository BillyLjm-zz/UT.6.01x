// Lab13.c
// Runs on LM4F120 or TM4C123
// Use SysTick interrupts to implement a 4-key digital piano
// edX Lab 13 
// Daniel Valvano, Jonathan Valvano
// March 13, 2014
// Port B bits 6-0 have the 6-bit DAC
// Port E bits 3-0 have 4 piano keys

#include "..//tm4c123gh6pm.h"
#include "Sound.h"
#include "Piano.h"
#include "TExaS.h"

// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void delay(unsigned long msec);

int main(void){ // for the real board grader to work, you must connect PD3 to your DAC output
  int piano; // to read current piano keys
	int prevPiano; // to store piano keys in prev cycle
	
	TExaS_Init(SW_PIN_PE3210, DAC_PIN_PB3210,ScopeOn); // activate grader and set system clock to 80 MHz  
  Sound_Init(); // initialize SysTick timer and DAC
  Piano_Init();
  EnableInterrupts();  // enable after all initialization are done
	
	while(1){ 
		piano = Piano_In();
	
		if(piano != prevPiano){ // only react if piano keys pressed
			if(piano == 0x00)
				Sound_Off();
			else if(piano == 0x01)
				Sound_Tone(2389);
			else if(piano == 0x02)
				Sound_Tone(2128);
			else if(piano == 0x04)
				Sound_Tone(1896);
			else if(piano == 0x08)
				Sound_Tone(1594);
			delay(10);
		}
		
		prevPiano = piano;
	}
}

// Inputs: Number of msec to delay
// Outputs: None
void delay(unsigned long msec){ 
  unsigned long count;
  while(msec > 0 ) {  // repeat while there are still delay
    count = 16000;    // about 1ms
    while (count > 0) { 
      count--;
    } // This while loop takes approximately 3 cycles
    msec--;
  }
}


