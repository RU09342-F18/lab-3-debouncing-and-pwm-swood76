#include <msp.h>
void ButtonSetup();
void LEDSetup();
void TimerA0Setup();

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Disables the watchdog timer, since we do not want it to reset the MCU when an infinite loop occurs

     //Function call(s)

    ButtonSetup();
    LEDSetup();
    TimerA0Setup();

    __enable_interrupt();
}
void ButtonSetup()
{
            P1REN  |= BIT1;  // Initializes the pull up or down resistor that is included in P1.1;
            P1OUT  |= BIT1;  //Configures the pull up or pull down resistor as a pull up resistor for P1.1 since the button is desired to be an input when the button is pressed, an the PWM's duty cycle increases
                             // logic high is read when the button is not pressed since the pull up is connected directly to VDD, and a logic zero is read when the button is pressed
            P1IES &= ~BIT1; // Configure Interrupt to occur on the low to high state transition
            P1IE |= BIT1;   //Configures interrupt to be high on the P1IE register so when button is pressed; the PORT1 ISR is enabled
            P1IFG &= ~BIT1; // P1IFG is set to zero to verify the event; the button press has not occurred, so ISR can run

}

void LEDSetup()
{
                P1DIR  |= BIT0; // Initialize and set the port1 direction for BIT0, such that P1.0 is an output, and
                // that the GREEN LED is set as an output
}

void TimerA0Setup()
{
    TA0CCTL0 = CCIE;  // Control (CCR0) register set to interrupt when on count of timer
    TA0CCR0 = 32768;  // Set value for Timer A0 to count up to, set to interval counter of 1 Hz, since 32,768/32,768 = 1
    TA0CTL = TASSEL_2 + TACLR; // Selection of clock to be submain clock, and clear timerA register


}




// Port 1 ISR function is to enable interrupt on the press of P1.3 such that the Green LED is toggled on and off
// at a delay caused by the submain clock
void Port1_IsrHandler(void)
{



    TA0CTL = TASSEL_2 + MC_1;           // Selection and configuration of the submain clock such that it is in Up mode
                                                     // set to count to CCR0
            P1OUT ^= BIT0; // Green LED (P1.0) is toggled

}

// TIMER_A0 ISR meant to reset interrupt caused and clear TimerA0
void TA0_IsrHandler(void)
{
            P1IE |= BIT1;   // Configured interrupt to occur on the press of button P1.3
           P1IFG &= ~BIT1; // P1IFG is set to zero to verify the event; the button press has not occurred, so ISR can run
           TA0CTL = TACLR; // Timer is cleared
}




