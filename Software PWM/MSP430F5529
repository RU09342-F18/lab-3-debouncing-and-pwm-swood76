/*
 * This code implements a 1kHz PWM signal with a duty cycle between 0% and 100%. Initially a 50 % duty cycle LED is generated.
 With the utilization of the PORT2 interrupt, and TIMERA1 interrupt, the press of a button P2.1 allows for the PWM to increase between
 0% and 100% duty cycle within respective button presses.
 Code is meant to be implemented and programmed to MSP430F5529 Launchpad
 *
 */



#include <msp430.h>

/*
 * Initialization of Global constants
 */
int ButtonEdge = 0; // Set state of button to zero for proper button debounce
int PWM = 500; // Initialized PWM as global constant of 500, so that on board LED duty cycle of 50 percent is achieved initially

/*
 *  Declaration of Functions Utilized
 */
void TimerA0Setup();
void TimerA1Setup();
void LEDSetup();
void ButtonSetup();

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD; // Disables the watchdog timer, since we do not want it to reset the MCU when an infinite loop occurs
    /*
     * Function call for the initialized variables utilized in each Timer, Led, and Button function
     */
    TimerA0Setup();
    TimerA1Setup();
    LEDSetup();
    ButtonSetup();



                // Infinite while loop, which allows the PWM to occur
                while(1)
                {
                    //when timer A's counter register is less than the desired PWM value of 1000, the LED is toggled at the desired duty cycle of 50 percent
                    // Since the PWM was set to a constant 500, the duty cycle is obtained by simply dividing by the desired PWM, the value stored in TA0CCR0 by 100
                    if(TA0R <= PWM)
                  {

                             P1OUT ^= BIT0;
                  }
                    //Once the counter is incremented past 1000, the LED is turned off, on the next button press
                    else if(TA0R > PWM)
                     {
                                P1OUT &= ~BIT0;
                     }
                 }
}

// Function which includes declaration of TimerA0 and the
// required selections for the desired PWM of 1 kHz
void TimerA0Setup()
{
        TA0CCR0 = 1000; // Value at which the timerA0 will count to and then reset; desired PWM
        TA0CTL = TASSEL_2 + MC_1 + TACLR;   // control of timer is selected to enable TimerAs clock source to subMaster Clock
                                            // and that the desired value is counted to by selecting Up Mode since
                                            // it counts to the desired PWM smclock; as well as clearing the TimerA register.
}

//Initialization of TimerA1
void TimerA1Setup()
{
    TA1CCR0 = 50000;   //Value at which the timerA1 will count to and then reset, so that when 10ms is reached debounce is controlled
    TA1CCTL0 = CCIE;   // Initialization of interrupts to occur when button is pressed so that the duty
                                   // cycle of 0% to 100 % is achieved
 }


//Initialization of the port and its respective LED, the Green LED. P1.0
void LEDSetup()
{
        P1DIR |= BIT0; // Set LED as an Output
        P1OUT ^= BIT0; // Set the LED initially on

}
// Function which includes the necessary initialized variables of button 3 P1.3
void ButtonSetup()
{
    //button configure
        P2DIR &= ~BIT1; // Initializes Button (P2.1) as an input
        P2REN |= BIT1;    // Initializes the pull up or down resistor that is included in P2.1;
        P2OUT |= BIT1;   //Configures the pull up or pull down resistor as a pull up resistor for P2.1 since the button is desired to be an input when the button is pressed, an the PWM's duty cycle increases
                          // logic high is read when the button is not pressed since the pull up is connected directly to VDD, and a logic zero is read when the button is pressed
        P2IE |= BIT1;  //Configures interrupt to be high on the P2IE register so when button is pressed; the PORT2 ISR is enabled
        P2IES |= BIT1; // Configures interrupt to occur when the button transition from a state of high to low, so when the TOGGLE INTERRUPT EDGE: HIGH TO LOW
        P2IFG &= ~BIT3; // P2IFG is set to zero to verify the event, the button press has  not occurred, so ISR can run

        __bis_SR_register(GIE); // Enables global interrupts

}



#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer_A0 (void)
{
            // Verify if button is not pressed, when low LED is off
            if(ButtonEdge == 0)
         {
                // Initially LED is off, and if after button is pressed the pulse width is less than the desired PWM value of 1000;
                // then then the pulse width is incremented by 100 on each button press
                if(PWM < 1000)
                {

                    PWM = PWM + 100;
                }

                // Once when the Button is pressed to the desired PWM value of 1000, an overflow occurs,
                // and the LED is turned off and the PWM is reset to zero
                else if(PWM == 1000)
                 {
                        PWM = 0;
                 }



                P2IE |= BIT1; // Re-Configures interrupt to be high on the P2IE register so when button is pressed; the PORT2 ISR is enabled
                P2IES &= ~BIT1; // Configure Interrupt to occur on the low to high state transition
                TA1CTL &= ~TASSEL_2; // TimerA1's Control register set to disable the submaster clock
                TA1CTL |= TACLR;  // TimerA1"s control register is cleared and reset


         }


            else if(ButtonEdge == 1)
                {
                        P2IE |= BIT1;  // Re-Configures interrupt to be high on the P2IE register so when button is pressed; the PORT1 ISR is enabled
                        P2IES |= BIT1;    // Configure Interrupt to occur on the high to low state transition since button has been pressed
                        P2IFG &= ~BIT1; // interrupt has not occurred, flag is cleared
                        TA1CTL &= ~TASSEL_2; // TimerA1's Control register set to disable the submaster clock
                        TA1CTL |= TACLR;    // TimerA1"s control register is cleared and reset

                }

}

#pragma vector=PORT2_VECTOR
__interrupt void PORT_2(void)
{
        // when button is not pressed. and will be turned on
        if(ButtonEdge == 0)
        {
            TA1CTL = TASSEL_2 + MC_1 + TACLR; // control of timerA1 is selected to enable TimerAs clock source to subMaster Clock
                                              // and that the desired value is counted to by selecting Up Mode since
                                              // it counts up to the desired PWM smclock; as well as clearing the TimerA register.
            P2IFG &= ~BIT1; // To verify button has not been pressed and interrupt has not occurred, flag is cleared

        }
        // when state of button is high and will be off
        else if(ButtonEdge == 1) //ON -> GOING OFF
        {
            TA1CTL = TASSEL_2 + MC_1 + TACLR; // control of timerA1 is selected to enable TimerAs clock source to subMaster Clock
                                              // and that the desired value is counted to by selecting Up Mode since
            P2IFG &= ~BIT1; // To verify button has been pressed and interrupt has occurred, flag is cleared

        }
 }




