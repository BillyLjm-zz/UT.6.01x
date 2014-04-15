// TExaS.h
// Runs on LM4F120/TM4C123
// Periodic timer Timer4A, ADC1, PD3, and UART0 used to create the scope
// Periodic timer Timer5A which will interact with debugger and grade the lab 
// It initializes on reset and runs whenever interrupts are enabled
// Jonathan Valvano
// March 4, 2014

/* This example accompanies the book
   "Embedded Systems: Real Time Operating Systems for ARM Cortex M Microcontrollers",
   ISBN: 978-1466468863, Jonathan Valvano, copyright (c) 2013
   Section 6.4.5, Program 6.1

 Copyright 2013 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

// IMPORTANT: These enumerated types are specific to Lab 13.
enum InputPorts{
  SW_PIN_PA5432 = 0,
  SW_PIN_PB3210 = 1,
  SW_PIN_PE3210 = 4
};

enum OutputPorts{
  DAC_PIN_PA5432 = 0, // optionally, you can use PA7-2, but the top two bits are not graded
  DAC_PIN_PB3210 = 1, // optionally, you can use PB5-0, but the top two bits are not graded
  DAC_PIN_PE3210 = 4  // optionally, you can use PE5-0, but the top two bits are not graded
};

enum ScopeModes{
  ScopeOff = 0,   // no scope needed
  ScopeOn = 1     // turn on UART0 and stream analog data from PD3 to TExaSdisplay
};
// ************TExaS_Init*****************
// Initialize grader, triggered by timer 5A
// This needs to be called once 
// Inputs: iport input(s) connected to this port
//         oport output(s) connected to this port
//         scope true if oscilloscope to be implemented (uses UART0 and TExaSdisplay)
// Outputs: none
void TExaS_Init(enum InputPorts iport, enum OutputPorts oport, enum ScopeModes scope);

// ************TExaS_Stop*****************
// Stop the transfer 
// Inputs:  none
// Outputs: none
void TExaS_Stop(void);
