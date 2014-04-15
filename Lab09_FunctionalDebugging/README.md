#Lab9_FunctionalDebugging

## Purpose
Lean to implement a data dump for debugging.

## What It Does
Use built-in switch to flash the built-in LED at 10Hz or leave it off. 
Also dumps data with timestamp whenever switch input changes.

    if SW1 or SW2 is pressed
        flash LED at 10Hz (Red)
    else
    	turn LED off
    
    if PortF Data changes
        data dump with timestamp

## Files
__main.c__  
`main()` - Main function
