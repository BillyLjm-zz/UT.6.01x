/**********************************************
 * Sound.c
 * UT.6.01x lab 13 
 * Runs on LM4F120 or TM4C123, 
 * 
 * Outputs sine-wave to 4-bit DAC at PB0-PB3
 * 
 * coded by Billy.Ljm
 * 12 April 2014
 **********************************************/

// **************Sound_Init*********************
// Initialize Systick periodic interrupts
// Also initializes DAC
// Input: none
// Output: none
void Sound_Init(void);

// **************Sound_Tone*********************
// Change SysTick periodic interrupts to start sound output
// Input: interrupt period
//           Units of period are 12.5ns
//           Maximum is 2^24-1
//           Minimum is 256
// Output: none
void Sound_Tone(unsigned long period);


// **************Sound_Off*********************
// stop outputing to DAC
// Output: none
void Sound_Off(void);

