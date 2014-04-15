/**********************************************
 * Piano.c
 * UT.6.01x lab 13 
 * Runs on LM4F120 or TM4C123, 
 * 
 * Implementation of 4 piano keys on PE0-PE3
 * 
 * coded by Billy.Ljm
 * 12 April 2014
 **********************************************/

// **************Piano_Init*********************
// Initialize piano key inputs
// Input: none
// Output: none
void Piano_Init(void); 
  

// **************Piano_In*********************
// Input from piano key inputs
// Input: none 
// Output: 0 to 15 depending on keys
// 0x01 is key 0 pressed, 0x02 is key 1 pressed,
// 0x04 is key 2 pressed, 0x08 is key 3 pressed
unsigned long Piano_In(void);
