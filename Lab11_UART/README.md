#Lab11_UART

## Purpose
Learn to use UART 


## What It Does
Uses UART to output manipulated decimal numbers


## Functions
__UART.c__  
`UART_OutString()` - extends `UART_OutChar()` to output strings  
`UART_ConvertUDec()` - converts decimal number to ascii numbers (max 4 digits)  
`UART_ConvertDistance()` -  divides decimal number by 10^4 then converts to ascii numbers