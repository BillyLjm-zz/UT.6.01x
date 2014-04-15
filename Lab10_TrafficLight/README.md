#Lab10_TrafficLight  

## Purpose
Learn to implement a finite state machine & use SysTick.  


## What It Does
Simulates the traffic light of a traffic junction.  
Has 3 vehicle/pedestrian direction, of which only 1 direction will be green at any time.   
Uses buttons as vehicle/pedestrian sensor, and LEDs as traffic lights.

    if 1 direction wants to cross
        stop other directions, allow present direction to move
    else
        rotate among present directions

## Functions
__TableTrafficLight.c__  
`ports_Init()` - Initialise all the ports  
`SysTick_Wait()` - Extends `SysTick_Wait10ms()` to wait for longer periods  
`main()` - Main function

__FSM_Table.pdf__ 

__CircuitDigram.jpg__