/**
 * Driver for ping sensor
 * @file ping.c
 * @author
 */

#include <inc/tm4c123gh6pm.h>
#include "ping.h"
#include "Timer.h"
#include "lcd.h"
#include <driverlib/interrupt.h>


volatile unsigned long START_TIME = 0;
volatile unsigned long END_TIME = 0;
volatile enum{LOW, HIGH, DONE} STATE = LOW; // State of ping echo pulse
bool overflow = false;
int numOverflow = 0;

void ping_init (void){


    SYSCTL_RCGCTIMER_R |= 0x8;
    SYSCTL_RCGCGPIO_R |= 0x2; //Enable Clock port B
    while((SYSCTL_PRGPIO_R & 0x2) != 0x2) {};
    GPIO_PORTB_DIR_R |= 0x8;
    GPIO_PORTB_AFSEL_R |= 0x8; //Enable alternate function
    GPIO_PORTB_DEN_R |= 0x8; //Enable pin 3 on port B
    GPIO_PORTB_PCTL_R |= 0x7000; //Set alternate function

    IntRegister(INT_TIMER3B, TIMER3B_Handler);

    IntMasterEnable();

    // Configure and enable the timer
    TIMER3_CTL_R &= ~0x100; //Disable Timer
    TIMER3_CFG_R = 0x4;
    TIMER3_TBMR_R = 0x7;
    TIMER3_CTL_R |= 0xC00;
    TIMER3_TBILR_R = 0xFFFF;
    TIMER3_TBPR_R = 0xFF;
    TIMER3_ICR_R = 0x400;
    TIMER3_IMR_R |= 0x400;
    NVIC_PRI9_R = 0xE0;
    NVIC_EN1_R = 0x10;
    TIMER3_CTL_R |= 0x100;

}

void ping_trigger (void){ //activate ping sensor
    STATE = LOW;
    // Disable timer and disable timer interrupt
    TIMER3_CTL_R &= ~0x100;
    TIMER3_IMR_R &= ~0X400;
    // Disable alternate function (disconnect timer from port pin)
    GPIO_PORTB_AFSEL_R &= 0xF7;

    GPIO_PORTB_DATA_R &= 0xF7;
    timer_waitMillis(0.005);
    GPIO_PORTB_DATA_R |= 0x8;
    timer_waitMillis(0.005);
    GPIO_PORTB_DATA_R &= 0xF7;


    // Clear an interrupt that may have been erroneously triggered
    TIMER3_ICR_R = 0X400;
    // Re-enable alternate function, timer interrupt, and timer
    GPIO_PORTB_AFSEL_R |= 0x8;
    TIMER3_IMR_R |= 0x400;
    TIMER3_CTL_R |= 0x100;
}

void TIMER3B_Handler(void){


    if (TIMER3_MIS_R & 0x400){

        TIMER3_ICR_R |= 0x400;
        overflow = false;

        if(STATE == LOW){
            START_TIME = TIMER3_TBR_R;
            STATE = HIGH;
        }
        else if (STATE == HIGH){
            END_TIME = TIMER3_TBR_R;
            STATE = DONE;
        }
    }
}

  // As needed, go back to review your interrupt handler code for the UART lab.
  // What are the first lines of code in the ISR? Regardless of the device, interrupt handling
  // includes checking the source of the interrupt and clearing the interrupt status bit.
  // Checking the source: test the MIS bit in the MIS register (is the ISR executing
  // because the input capture event happened and interrupts were enabled for that event?
  // Clearing the interrupt: set the ICR bit (so that same event doesn't trigger another interrupt)
  // The rest of the code in the ISR depends on actions needed when the event happens.



float ping_getDistance (void){

    ping_trigger();


    int width = START_TIME - END_TIME;

    if (width < 0){
      width = END_TIME - width;
      numOverflow++;
    }

    double distmm = width/16000.0; //distance in mm and time in ms
    double distcm = distmm * 34.3; //Convert to cm

    lcd_printf("Pulse Width: %d\nTime(ms): %.3lf\nDistance(cm): %.2lf\nNumber of OF: %d", width, distmm,distcm, numOverflow);

}
