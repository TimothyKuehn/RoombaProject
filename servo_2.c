#include "servo.h"

void servo_init(void)
{
    SYSCTL_RCGCGPIO_R |= 0x02;
    while ((SYSCTL_PRGPIO_R & 0x2) == 0)
    {
    }
    GPIO_PORTB_DIR_R |= 0x20;
    GPIO_PORTB_DEN_R |= 0x20;
    GPIO_PORTB_AFSEL_R |= 0x20;
    GPIO_PORTB_PCTL_R |= 0x700000;
    SYSCTL_RCGCTIMER_R |= 0x02;
    TIMER1_CTL_R &= ~0x100;
    TIMER1_CFG_R |= 0x4;
    TIMER1_TBMR_R |= 0xA;
    TIMER1_CTL_R &= ~0x4000;
    TIMER1_TBPR_R |= 0x04;
    TIMER1_TBILR_R |= 0xE200;
    TIMER1_CTL_R |= 0x100;

}
void servo_move(uint16_t degrees)
{
    TIMER1_CTL_R &= ~0x100;
    int deg = ((degrees - 170)*-89) + 304000;
    TIMER1_TBMATCHR_R = deg & 0xFFFF;
    TIMER1_TBPMR_R = deg >> 16;
    TIMER1_CTL_R |= 0x100;

}
