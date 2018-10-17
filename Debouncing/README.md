# Software Debouncing
Included in this repository is two examples of a program which is implemented on two TI launchpads: MSP430G2553 and MSP432P401R. This program is meant to solve the issue of button debouncing, and achieves this functinality by utilizing two interrupts PORT1 and TIMERXX such that when the button is pressed for each board, the port1 ISR is ran, and the respective LED is turned on, and the time kept in specific state is counted and delayed with TIMER interrupt so that debouncing effect is created, and the single press appears as it should: a single press. As of right now, the program is not at full functionality for the MSP432P401R, and there still exists some bugs, which limiting a successful implementation, but it functios correctly on the MSP430G25553. More information regarding the detail and  functionality of the program can be found in the main.c file for each board.

# Author
Scott Wood
