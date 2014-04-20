/********************************
 * Driver functions for an ADC
 *
 * Runs on LM4F120/TM4C123
 * Done as homework for UT.6.01x
 *
 * coded by Billy.Ljm
 * 19 April 2014
 * MIT LICENSE
 *******************************/

// Intialises ADC on PE2
void ADC0_Init(void); 
  
// Busy-wait reading of ADC
// Returns: 12-bit result of ADC conversion
unsigned long ADC0_In(void);
