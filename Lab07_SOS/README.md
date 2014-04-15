#Lab7_SOS    

## Purpose
Learn to create a real-time system.

## What It Does
Use the launchpad's built-in switches (SW1 & SW2) to use the built-in LED to flash SOS in yellow light. 

    if SW1 & SW2 is pressed
        flash SOS (yellow)

## Functions
__main.c__  
`PortF_Init()` - Initialises Port F  
`FlashSOS()` - Flash yellow SOS  
`Main loop` - Read switches & call FlashSOS() if needed