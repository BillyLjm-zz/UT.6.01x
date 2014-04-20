/*******************************
 * Driver functions for an ADC
 *
 * Runs on LM4F120/TM4C123
 * Done as homework for UT.6.01x
 *
 * coded by Billy.Ljm
 * 19 April 2014
 * MIT LICENSE
 ********************************/

#include "ADC.h"
#include "..//tm4c123gh6pm.h"

void ADC0_Init(void){ 
  // Intialises ADC on PE2
	unsigned long volatile delay;
	SYSCTL_RCGC2_R |= 0x10; 				// activate port E
  delay = SYSCTL_RCGC2_R;
  GPIO_PORTE_DIR_R &= ~0X04;     	// make PE2 in
	GPIO_PORTE_AFSEL_R |= 0X04;  		// enable alt funct on PE2
	GPIO_PORTE_DEN_R &= ~0X04;     	// disable digital I/O on PE2
	GPIO_PORTE_AMSEL_R |= 0x04;     // enable analog on PE2
	
	SYSCTL_RCGC0_R |= 0x00010000;   // activate ADC0
	delay = SYSCTL_RCGC2_R;        
	SYSCTL_RCGC0_R &= ~0x00000300;  // 125K samplig rate
	ADC0_SSPRI_R = 0x0123;          // SS3 = highest priority
	ADC0_ACTSS_R &= ~0x0008;        // disable SS3
	ADC0_EMUX_R &= ~0xF000;         // software trigger
	ADC0_SSMUX3_R &= ~0x000F;       // clear SS3 
	ADC0_SSMUX3_R += 1;             // set channel to Ain1/PE2
	ADC0_SSCTL3_R = 0x0006;         // no TS0 D0, yes IE0 END0
	ADC0_ACTSS_R |= 0x0008;         // enable SS3
}


unsigned long ADC0_In(void){  
	// Busy-wait reading of ADC
	// Returns: 12-bit result of ADC conversion
	unsigned long result;
	ADC0_PSSI_R = 0x0008;
	while((ADC0_RIS_R&0x08)==0){};
	result = ADC0_SSFIFO3_R&0xFFF;
	ADC0_ISC_R = 0x0008;
	return result;
}
