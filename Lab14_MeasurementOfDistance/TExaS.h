// TExaS.h
// Runs on LM4F120/TM4C123
// Periodic timer Timer5A which will interact with debugger and grade the lab 
// It initializes on reset and runs whenever interrupts are enabled
// Jonathan Valvano. Daniel Valvano
// April 8, 2014


// IMPORTANT: These enumerated types are specific to Lab 14.
enum InputPorts{
  ADC0_AIN1_PIN_PE2 = 2, // used again in Lab 15
  ADC0_AIN0_PIN_PE3 = 3,
  ADC0_AIN8_PIN_PE5 = 5
};


// You have four options for display drivers/hardware:
// 1) Emulate the Nokia5110 LCD using UART0 and PA1-0 to send data to a PC
//    running TExaSdisplay in Nokia mode. (Note: this mode is not finished yet)
// 2) Interface a real Nokia5110 LCD and use SSI0 and PA7-2 to send data and
//    commands to it.  Allow TExaS to use UART0 and a PC running TExaSdisplay
//    to implement a low-cost oscilloscope.
// 3) Interface a real Nokia5110 LCD and use SSI0 and PA7-2 to send data and
//    commands to it.  Do not enable the oscilloscope.
// 4) There is no LCD. Do not enable the oscilloscope. You may use this mode to
//    have no display or to use the UART ASCII output like Labs 5 and 11
enum DisplayType{
  UART0_Emulate_Nokia5110_NoScope,  // not finished yet
  SSI0_Real_Nokia5110_Scope,
  SSI0_Real_Nokia5110_NoScope,
  NoLCD_NoScope
};

// ************TExaS_Init*****************
// Initialize grader, triggered by timer 5A
// This needs to be called once 
// Inputs: iport   input(s) connected to this port
//         display system used to output the results
// Outputs: none
void TExaS_Init(enum InputPorts iport, enum DisplayType display);

// ************TExaS_Stop*****************
// Stop the transfer 
// Inputs:  none
// Outputs: none
void TExaS_Stop(void);
