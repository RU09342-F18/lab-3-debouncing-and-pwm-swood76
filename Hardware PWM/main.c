#include <msp430fr2311.h>


int PWM = 500;
/*
 * Declaration of functions utilized
 */
void LEDSetup();
void TimerB0Setup();
void TimerB1Setup();
void  ButtonSetup();

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD; // Disable the watchdog timer from being active, so it does not reset MCU/program
    PM5CTL0 &= ~LOCKLPM5;  //Disables the GPIO power-on default high-impedance mode to activate previously configured port settings (TI)",
                            //this is due to the fact that LOCKLPM5 is set to enable a lock that is within the memory mapped output register

    /*
             Function calls
    */
    LEDSetup();
    TimerB0Setup();
    TimerB1Setup();
    ButtonSetup();
    __bis_SR_register(LPM0_bits + GIE); // Enables Low Power Mode0 as well as enabling global interrupts
    // Entering Low Power Mode0 enables it so the only necessary clocks are active: SMCLK and ACLK, as well as disabling
    // CPU and MCLK

    return 0;
}

void LEDSetup()
{

        P2DIR |= BIT0; // Since P2.0 can be figured as output for TimerB1,
                       //P2.0 is set as an output
        P2SEL0 |= BIT0; // Configures Output pin as a special function
                       // such that it is an output for TimerB1,
                       // without this line, nothing would happen because
                       // the LED would not be connected to the Timer
}

/*
 * Utilize TimerB since for the MSP430FFR2311, the output pin P2.0 LED2 for PWM is connected to TIMERB1
 */
void TimerB1Setup()
{
        TB1CTL = TBSSEL__SMCLK + MC__UP + TBCLR + ID_2;           //Selects source to be submaster clock, up Mode; able to still do so in Low Power Mode0
        TB1CCR0 = 1000-1;                   //Period we want the PWM to happen at such it will count to 999; compensates for zero by 1000-1
        TB1CCR1 = 500;                      //Pulse width of PWM to start at 50 %, this will be reached first since
                                             // in output Mode 7, and in general, the CCR1 register is reached
                                             // prior to the CCR0 register, in this configuration the pin remains high until CCR1 is reached and low until CCR0 is reached
                                             // changing the value of CCR1 and keeping CCR0 constant, the pulse width can be changed
        TB1CCTL1 = OUTMOD_7;                // TimerB0 Control Register set to output mode 7,
                                                   // which is reset/set and is normally used for PWM

}
void TimerB0Setup()
{
        TB0CCR0 = 10000; //Value TimerB0 CCR0 register set to count to and interrupt overflow
        TB0CCTL0 = CCIE;  // Allows for interrupt to be enabled when the value of CCR1 is reached

}

void ButtonSetup()
{

        P1DIR &= ~BIT1;  //Initialization of button (P1.1) as an input,
        P1OUT |= BIT1;   // /Configures the pull up or pull down resistor as a pull up resistor for P1.1 since the button is desired to be an input when the button is pressed, an the PWM's duty cycle increases
                                // logic high is read when the button is not pressed since the pull up is connected directly
                                //  to VDD, and a logic zero is read when the button is pressed
        P1REN |= BIT1;   //Initialization of the pull up or pull down resistor that is connected to the button

        P1IE |= BIT1;   //Configures interrupt to be high on the P1IE register so when button is pressed;
                        //the PORT1 ISR is enabled, without button has no effect on LED

}

//Timer0_B0 ISR
#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_b0 (void)
{
   // when the desired PWM value of 1000 is not reached the PWM is incremented by 100 until 1000 is reached
    if(PWM < 1000)
    {
      PWM = PWM + 100;
    }
    // reset PWM when desired PWM value is reached
    else
    {
        PWM = 0;
    }
    P1IE |= BIT1;//Configures interrupt to be high on the P1IE register so when button is pressed; the PORT1 ISR is enabled
    TB0CTL = 0; // TimerA1's Control register set to disable the submaster clock
    TB1CCR1 = PWM;// //Pulse width of PWM set to start at 50 %,

}
// PORT1 ISR
#pragma vector = PORT1_VECTOR
__interrupt void PORT_1(void)
{
    P1IFG &= ~BIT1;  // P1IFG is set to zero to verify the event; the button press has not occurred, so ISR can run
    TB0CTL = TBSSEL__SMCLK + MC__UP + TBCLR + ID_2; // When button pressed and port1 ISR is ran, timerB0's control is set
                              // to submaster clock, in Up mode, if line not present no count occurs and LED just
}
