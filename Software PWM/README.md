# Software PWM
Included in this repository is two examples of implementing Software PWM on two TI launchpads: MSP430G2553 and MSP430F5529. This was accomplished by implementing two interrupts: PORT1 and TIMERXX such that after the button is pressed, the on-board LED, which is initially at a duty cyle of 50%, is incremented between a range of 0% to 100% in steps of 10%, so that after the button is pressed, it is slowly increased until it reaches 100%, and resets. This program takes advantage of the fact that it does not rely on the special function register of each microprocessors to allow for the PWM operation mode of their respective TIMER modules.


# Author
Scott Wood
