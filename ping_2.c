/*
 * ping.c
 */

#include <stdio.h>
#include <stdint.h>
#include <inc/tm4c123gh6pm.h>
#include <timer.h>
#include <driverlib/interrupt.h>
volatile unsigned long START_TIME = 0;
volatile unsigned long END_TIME = 0;
volatile enum
{
    LOW, HIGH, DONE
} STATE = LOW; // State of ping echo pulse

void TIMER3B_Handler(void)
{
    if (TIMER3_MIS_R & 0x400)
    {
        TIMER3_ICR_R |= 0x400;
        if (STATE == LOW)
        {
            START_TIME = TIMER3_TBR_R;
            STATE = HIGH;
        }
        else if (STATE == HIGH)
        {
            END_TIME = TIMER3_TBR_R;
            STATE = DONE;
        }
    }

}

void ping_init(void)
{ //initialize the timer

    SYSCTL_RCGCGPIO_R |= 0x02; //enable clock to port B
    GPIO_PORTB_DIR_R |= 0x08;
    GPIO_PORTB_DEN_R |= 0x08; //enable pin 3 on port B
    GPIO_PORTB_AFSEL_R |= 0x08; //enable alternate function to pin 3
    GPIO_PORTB_PCTL_R |= 0x7000; //set the alternate function of pin 3 to 7

    SYSCTL_RCGCTIMER_R |= 0x08;
    TIMER3_CTL_R &= ~0x100; //disable the timer while it is being configured
    TIMER3_CFG_R |= 0x00000004; // set the time to 16-bit mode
    TIMER3_TBMR_R |= 0x07; //Edge-time mode & capture mode
    TIMER3_TBMR_R &= ~0x10; //count down
    TIMER3_CTL_R |= 0xC00;
    TIMER3_TBPR_R |= 0xFF; //sets a 16-bit max value
    TIMER3_TBILR_R |= 0xFFFF;
    TIMER3_ICR_R |= 0x400; //Clear capture interrupt flag
    TIMER3_CTL_R |= 0x100;
    NVIC_EN1_R |= 0x10;
    NVIC_PRI9_R |= 0x20;

    IntRegister(INT_TIMER3B, TIMER3B_Handler);
    IntMasterEnable(); //allow CPU to service interrupts

}

void ping_trigger(void)
{ //activate the PING sensor
    STATE = LOW;
    TIMER3_CTL_R &= ~0x100; //disable the timer //~0x100;
    TIMER3_IMR_R &= ~0x400; //Mask timer interrupt
    GPIO_PORTB_AFSEL_R &= ~0x08; //Disable alternate function to PB3
    GPIO_PORTB_DIR_R |= 0x08; //set PB3 as an output
    GPIO_PORTB_DATA_R &= ~0x08;
    GPIO_PORTB_DATA_R |= 0x08; //set PB3 HigH
    timer_waitMicros(5); //wait 5 microseconds
//    GPIO_PORTB_DIR_R &= 0b11110111; //set PB3 as an input
    GPIO_PORTB_DATA_R &= ~0x08; //set PB3 low
    TIMER3_ICR_R |= 0x400; //Clear capture interrupt flag
    GPIO_PORTB_AFSEL_R |= 0x08; //reenable the alternate function to PB3
    TIMER3_IMR_R |= 0x400; //Unmask timer intterupt
    TIMER3_CTL_R |= 0x100; //enable the timer

}

float ping_getDistance(void)
{ //calculate distance
    ping_trigger();
    while (STATE != DONE)
    {
    }
    float value = START_TIME - END_TIME;
    if (END_TIME > START_TIME)
    {
        return ping_getDistance();
    }
    value = value / 16000;
    value = value / 1000 * 343;
    value = value / 2 * 100;
}
