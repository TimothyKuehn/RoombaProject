/*
 * servo.c
 *
 *  Created on: Apr 11, 2023
 *      Author: jtyost
 */
#include "servo.h"


void servo_init(void){

    SYSCTL_RCGCTIMER_R |= 0x2;
    SYSCTL_RCGCGPIO_R |= 0x2;

    while ((SYSCTL_PRTIMER_R & 0x2) == 0) {};
    while ((SYSCTL_PRGPIO_R & 0x2) == 0) {};

    GPIO_PORTB_DEN_R |= 0x20;
    GPIO_PORTB_AFSEL_R |= 0x20;
    GPIO_PORTB_PCTL_R |= 0x00700000;

    TIMER1_CTL_R &= ~0x100;
    TIMER1_CFG_R = 0x4;
    TIMER1_TBMR_R = 0xA;
    TIMER1_TBPR_R = 0x4;
    TIMER1_TBILR_R = 0xE200;
    TIMER1_TBPMR_R = 0x4;
    TIMER1_TBMATCHR_R = 0xA380;
    TIMER1_CTL_R |= 0x100;
    timer_waitMillis(1000);

}
uint16_t servo_move(uint16_t degrees, uint16_t last){

  double right = .45; //0 degrees

  double left = 2.2; //180 degrees

  double degreetolowtime = ((left - right)/ 180) * (double)degrees + right;

  double timeinMs = 20 - degreetolowtime;

  int cycles = timeinMs * 16000;

  TIMER1_TBPMR_R = 0x4;
  TIMER1_TBMATCHR_R = cycles - 0x40000;

  int waitTime = (abs(degrees - last)) * 10;
  timer_waitMillis(waitTime);

  return degrees;
}
