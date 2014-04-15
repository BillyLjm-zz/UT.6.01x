/**********************************************
 * DAC.c
 * UT.6.01x lab 13 
 * Runs on LM4F120 or TM4C123, 
 * 
 * Implementation of the 4-bit DAC on PB3-PB0
 * 
 * coded by Billy.Ljm
 * 12 April 2014
 **********************************************/

// **************DAC_Init*********************
// Initialize 4-bit DAC at PB0-PB3
// Input: none
// Output: none
void DAC_Init(void);


// **************DAC_Out*********************
// output to DAC
// Input: 4-bit data, 0 to 15
// Output: none
void DAC_Out(unsigned long data);
  


